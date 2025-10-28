#include "InputManager.hpp"
#include "../core/Config.hpp"

#include <spdlog/spdlog.h>
#include <entt/signal/dispatcher.hpp>
#include <entt/core/hashed_string.hpp>
#include <imgui.h>
#include <imgui_impl_sdl3.h>

#include <string>
#include <stdexcept>

namespace engine::input { 

InputManager::InputManager(SDL_Renderer *SDLRenderer, const engine::core::Config *config) : m_SDLRenderer(SDLRenderer) {
    // 检查SDL渲染器是否有效
    if (!SDLRenderer) {
        spdlog::error("INPUTMANAGER::SDL_Renderer 为空");  // 记录错误日志
        throw std::runtime_error("INPUTMANAGER::SDL_Renderer 为空");  // 抛出异常
    }
    initializeMappings(config);  // 初始化输入映射
    float x, y;
    SDL_GetMouseState(&x, &y);
    m_mousePosition = {x, y};
    spdlog::trace("INPUTMANAGER::SDL_Renderer 初始化成功, 鼠标位置: ({}, {})", x, y);
}

entt::sink<entt::sigh<void()>> InputManager::onAction(entt::id_type actionNameID, ActionState actionState) {
    // 如果actionName不存在，自动创建一个 std::array<...>
    // .at() 会进行边界检查，更安全
    return m_actionToFunc[actionNameID].at(static_cast<size_t>(actionState));
}

void InputManager::update() {
    // 1. 根据上一帧的值更新默认的动作状态
    for (auto& [actionName, state] : m_actionStates) {
        if (state == ActionState::PRESSED) state = ActionState::HELD;
        else if (state == ActionState::RELEASED) state = ActionState::INACTIVE;
    }
    // 2. 处理所有待处理的 SDL 事件 (这将设定 m_actionStates 的值)
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        processEvent(event);
    }
    // 3. 触发回调
    for (auto& [actionNameID, state] : m_actionStates) {
        if (state != ActionState::INACTIVE) {   // 如果动作状态不是 INACTIVE，
            if (auto it = m_actionToFunc.find(actionNameID); it != m_actionToFunc.end()) {
                it->second.at(static_cast<size_t>(state)).publish();
            }
        }
    }
}

void InputManager::quit() {
}

bool InputManager::isActionDown(entt::id_type actionNameID) const {
    if (auto it = m_actionStates.find(actionNameID); it != m_actionStates.end()) {
        return it->second == ActionState::PRESSED || it->second == ActionState::HELD;
    }
    return false;
}

bool InputManager::isActionPressed(entt::id_type actionNameID) const {
    if (auto it = m_actionStates.find(actionNameID); it != m_actionStates.end()) {
        return it->second == ActionState::PRESSED;
    }
    return false;
}

bool InputManager::isActionReleased(entt::id_type actionNameID) const {
    if (auto it = m_actionStates.find(actionNameID); it != m_actionStates.end()) {
        return it->second == ActionState::RELEASED;
    }
    return false;
}

glm::vec2 InputManager::getLogicalMousePosition() const {
    glm::vec2 logicalPos;
    // 通过窗口坐标获取渲染坐标（逻辑坐标）
    SDL_RenderCoordinatesFromWindow(m_SDLRenderer, m_mousePosition.x, m_mousePosition.y, &logicalPos.x, &logicalPos.y);
    return logicalPos;
}

void InputManager::processEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP: {
            SDL_Scancode scancode = event.key.scancode;     // 获取按键的scancode
            bool isDown = event.key.down; 
            bool isRepeat = event.key.repeat;

            auto it = m_inputToActions.find(scancode);
            if (it != m_inputToActions.end()) {     // 如果按键有对应的action
                const std::vector<entt::id_type>& associatedActions = it->second;
                for (const auto& actionName : associatedActions) {
                    updateActionState(actionName, isDown, isRepeat); // 更新action状态
                }
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            Uint32 button = event.button.button;  // 获取鼠标按钮
            bool isDown = event.button.down;
            auto it = m_inputToActions.find(button);
            if (it != m_inputToActions.end()) {     // 如果鼠标按钮有对应的action
                const std::vector<entt::id_type>& associatedActions = it->second;
                for (const auto& actionName : associatedActions) {
                    // 鼠标事件不考虑repeat, 所以第三个参数传false
                    updateActionState(actionName, isDown, false); // 更新action状态
                }
            }
            m_mousePosition = {event.button.x, event.button.y};  // 在点击时更新鼠标位置
            break;
        }
        case SDL_EVENT_MOUSE_MOTION:  // 处理鼠标运动
            m_mousePosition = {event.motion.x, event.motion.y};
            break;
        case SDL_EVENT_QUIT:
            m_shouldQuit = true;
            break;
        default:
            break;
    }
}

void InputManager::initializeMappings(const engine::core::Config* config) {
    spdlog::trace("INPUTMANAGER::initializeMappings::初始化输入映射...");
    if (!config) {
        spdlog::error("INPUTMANAGER::initializeMappings::Config 为空指针");
        throw std::runtime_error("INPUTMANAGER::initializeMappings::Config 为空指针");
    }
    auto actionsToKeyname = config->m_inputMappings;      // 获取配置中的输入映射（动作 -> 按键名称）
    m_inputToActions.clear();
    m_actionStates.clear();

    // 如果配置中没有定义鼠标按钮动作(通常不需要配置),则添加默认映射, 用于 UI
    if (actionsToKeyname.find("mouse_left") == actionsToKeyname.end()) {
         spdlog::debug("INPUTMANAGER::initializeMappings::配置中没有定义 'mouse_left' 动作,添加默认映射到 'MouseLeft'.");
         actionsToKeyname["mouse_left"] = {"MouseLeft"};     // 如果缺失则添加默认映射
    }
    if (actionsToKeyname.find("mouse_right") == actionsToKeyname.end()) {
         spdlog::debug("INPUTMANAGER::initializeMappings::配置中没有定义 'mouse_right' 动作,添加默认映射到 'MouseRight'.");
         actionsToKeyname["mouse_right"] = {"MouseRight"};   // 如果缺失则添加默认映射
    }

    // 遍历 动作 -> 按键名称 的映射
    for (const auto& [actionName, keyNames] : actionsToKeyname) {
        // 每个动作对应一个动作状态，初始化为 INACTIVE
        auto actionNameID = entt::hashed_string(actionName.c_str());
        m_actionStates[actionNameID] = ActionState::INACTIVE;
        spdlog::trace("INPUTMANAGER::initializeMappings::映射动作: {}", actionName);
        // 设置 "按键 -> 动作" 的映射
        for (const auto& keyName : keyNames) {
            SDL_Scancode scancode = scancodeFromString(keyName);       // 尝试根据按键名称获取scancode
            Uint32 mouse_button = mouseButtonFromString(keyName);  // 尝试根据按键名称获取鼠标按钮
            // 未来可添加其它输入类型 ...

            if (scancode != SDL_SCANCODE_UNKNOWN) {      // 如果scancode有效,则将action添加到scancode_to_actions_map_中
                m_inputToActions[scancode].push_back(actionNameID);     
                spdlog::trace("INPUTMANAGER::initializeMappings::  映射按键: {} (Scancode: {}) 到动作: {}", keyName, static_cast<int>(scancode), actionName);
            } else if (mouse_button != 0) {             // 如果鼠标按钮有效,则将action添加到mouse_button_to_actions_map_中
                m_inputToActions[mouse_button].push_back(actionNameID); 
                spdlog::trace("INPUTMANAGER::initializeMappings::  映射鼠标按钮: {} (Button ID: {}) 到动作: {}", keyName, static_cast<int>(mouse_button), actionName);
                // else if: 未来可添加其它输入类型 ...
            } else {
                spdlog::warn("INPUTMANAGER::initializeMappings::输入映射警告: 未知键或按钮名称 '{}' 用于动作 '{}'.", keyName, actionName);
            }
        }
    }
    spdlog::trace("INPUTMANAGER::initializeMappings::输入映射初始化完成.");
}

SDL_Scancode InputManager::scancodeFromString(std::string_view keyName) {
    return SDL_GetScancodeFromName(keyName.data());
}

Uint32 InputManager::mouseButtonFromString(std::string_view buttonName) {
    if (buttonName == "MouseLeft") return SDL_BUTTON_LEFT;
    if (buttonName == "MouseMiddle") return SDL_BUTTON_MIDDLE;
    if (buttonName == "MouseRight") return SDL_BUTTON_RIGHT;
    // SDL 还定义了 SDL_BUTTON_X1 和 SDL_BUTTON_X2
    if (buttonName == "MouseX1") return SDL_BUTTON_X1;
    if (buttonName == "MouseX2") return SDL_BUTTON_X2;
    return 0; // 0 不是有效的按钮值，表示无效
}

void InputManager::updateActionState(entt::id_type actionNameID, bool isInputActive, bool isRepeatEvent) {
    auto it = m_actionStates.find(actionNameID);
    if (it == m_actionStates.end()) {
        spdlog::warn("INPUTMANAGER::updateActionState::尝试更新未注册的动作状态: {}", actionNameID);
        return;
    }

    if (isInputActive) {
        if (isRepeatEvent) {
            it->second = ActionState::HELD;
        } else {
            it->second = ActionState::PRESSED;
        }
    } else {
        it->second = ActionState::RELEASED;
    }
}

}
