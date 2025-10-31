#pragma once
#include <SDL3/SDL_render.h>
#include <entt/signal/fwd.hpp>
#include <entt/signal/sigh.hpp>
#include <glm/vec2.hpp>

#include <array>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace engine::core {
class Config;
} // namespace engine::core

namespace engine::input {

enum class ActionState {
    PRESSED,
    HELD,
    RELEASED,
    INACTIVE
};

class InputManager final {
  private:
    SDL_Renderer     *m_SDLRenderer;          ///< @brief 用于获取逻辑坐标的 SDL_Renderer 指针
    entt::dispatcher *m_dispatcher;           ///< @brief 用于处理动作信号的调度器
    glm::vec2         m_mousePosition;        ///< @brief 鼠标在屏幕坐标中的位置
    glm::vec2         m_logicalMousePosition; ///< @brief 鼠标在逻辑坐标中的位置

    std::unordered_map<entt::id_type, std::array<entt::sigh<bool()>, 3>>               m_actionToFunc;   ///< @brief 从动作名称到关联的函数指针的映射
    std::unordered_map<std::variant<SDL_Scancode, Uint32>, std::vector<entt::id_type>> m_inputToActions; ///< @brief 从输入到关联的动作名称列表
    std::unordered_map<entt::id_type, ActionState>                                     m_actionStates;   ///< @brief 存储每个动作的当前状态

  public:
    InputManager(SDL_Renderer *SDLRenderer, const engine::core::Config *config, entt::dispatcher *dispatcher);

    void update();
    void quit(); ///< @brief 退出游戏

    /**
     * @brief 注册键盘动作
     * @param actionNameID 动作名称
     * @param actionState 动作状态
     * @param dispatcher 调度器
     * @return entt::sink<entt::sigh<bool()>> 动作信号
     */
    entt::sink<entt::sigh<bool()>> onAction(entt::id_type actionNameID, ActionState actionState = ActionState::PRESSED);
    // 保留动作状态检查, 提供不同的使用选择
    bool isActionDown(entt::id_type actionNameID) const;     ///< @brief 动作当前是否触发 (持续按下或本帧按下)
    bool isActionPressed(entt::id_type actionNameID) const;  ///< @brief 动作是否在本帧刚刚按下
    bool isActionReleased(entt::id_type actionNameID) const; ///< @brief 动作是否在本帧刚刚释放

    glm::vec2 getMousePosition() const { return m_mousePosition; }; ///< @brief 获取鼠标位置 （屏幕坐标）
    glm::vec2 getLogicalMousePosition() const;                      ///< @brief 获取鼠标位置 （逻辑坐标）

  private:
    void processEvent(const SDL_Event &event);                   ///< @brief 处理 SDL 事件（将按键转换为动作状态）
    void initializeMappings(const engine::core::Config *config); ///< @brief 根据 Config配置初始化映射表

    void         updateActionState(entt::id_type actionNameID, bool isInputActive, bool isRepeatEvent); ///< @brief 辅助更新动作状态
    SDL_Scancode scancodeFromString(std::string_view keyName);                                          ///< @brief 将字符串键名转换为 SDL_Scancode
    Uint32       mouseButtonFromString(std::string_view buttonName);                                    ///< @brief 将字符串按钮名转换为 SDL_Button
};

} // namespace engine::input
