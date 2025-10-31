#pragma once
#include <entt/signal/fwd.hpp>

namespace engine::input {
class InputManager;
}

namespace engine::render {
class Renderer;
class Camera;
class TextRenderer;
}

namespace engine::resource {
class ResourceManager;
}

namespace engine::core {
class GameState;

/**
 * @class Context
 * @brief 引擎核心上下文类，提供对引擎主要组件的统一访问接口
 *
 * Context类采用设计模式中的门面模式(Facade Pattern)，为引擎的各个子系统
 * (如输入管理、渲染、资源管理等)提供一个统一的访问点。它持有对各个子系统
 * 的引用，并提供简单的getter方法供其他模块使用。
 *
 * 该类是不可拷贝和不可移动的，确保只有一个上下文实例存在。
 */
class Context final {
  private:
    entt::dispatcher &m_dispatcher;                       ///< 事件分发器
    engine::input::InputManager &m_inputManager;          ///< 输入管理器引用
    engine::render::Renderer &m_renderer;                 ///< 渲染器引用
    engine::render::Camera &m_camera;                     ///< 相机引用
    engine::render::TextRenderer &m_textRenderer;         ///< 文本渲染器引用
    engine::resource::ResourceManager &m_resourceManager; ///< 资源管理器引用
    engine::core::GameState &m_gameState;                 ///< 游戏状态

  public:
    /**
     * @brief 构造函数，初始化上下文
     * @param dispatcher 事件分发器引用
     * @param inputManager 输入管理器引用
     * @param renderer 渲染器引用
     * @param camera 相机引用
     * @param textRenderer 文本渲染器引用
     * @param resourceManager 资源管理器引用
     * @param gameState 游戏状态引用
     */
    Context(
        entt::dispatcher &dispatcher,
        engine::input::InputManager &inputManager,
        engine::render::Renderer &renderer,
        engine::render::Camera &camera,
        engine::render::TextRenderer &textRenderer,
        engine::resource::ResourceManager &resourceManager,
        engine::core::GameState &gameState);

    Context(const Context &) = delete;
    Context &operator=(const Context &) = delete;
    Context(Context &&) = delete;
    Context &operator=(Context &&) = delete;

    entt::dispatcher &getDispatcher() const { return m_dispatcher; }                            ///< @brief 获取事件分发器
    engine::input::InputManager &getInputManager() const { return m_inputManager; }             ///< @brief 获取输入管理器
    engine::render::Renderer &getRenderer() const { return m_renderer; }                        ///< @brief 获取渲染器
    engine::render::Camera &getCamera() const { return m_camera; }                              ///< @brief 获取相机
    engine::render::TextRenderer &getTextRenderer() const { return m_textRenderer; }            ///< @brief 获取文本渲染器
    engine::resource::ResourceManager &getResourceManager() const { return m_resourceManager; } ///< @brief 获取资源管理器
    engine::core::GameState &getGameState() const { return m_gameState; }                       ///< @brief 获取游戏状态
};

} // namespace engine::core