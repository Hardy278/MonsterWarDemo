/**
 * @file SceneManager.h
 * @brief 场景管理器的实现，负责场景的切换和管理
 */

#pragma once
#include <memory>
#include <string>
#include <vector>
#include "../utils/Events.hpp"

// 前置声明
namespace engine::core {
    class Context;
}
namespace engine::scene {
    class Scene;
}

namespace engine::scene {

/**
 * @class SceneManager
 * @brief 场景管理器类，用于管理游戏场景的生命周期和切换
 * 
 * SceneManager 使用栈结构管理场景，支持场景的压入、弹出和替换操作。
 * 所有场景切换操作都是线程安全的，使用延时处理机制确保场景切换不会中断当前帧的渲染。
 * 该类被声明为 final，禁止被继承。
 */
class SceneManager final {
private:
    /**
     * @enum PendingAction
     * @brief 待处理的场景操作类型
     */
    enum class PendingAction { None, Push, Pop, Replace };

    engine::core::Context& m_context;                       ///< 引擎上下文引用，提供全局系统服务
    std::vector<std::unique_ptr<Scene>> m_sceneStack;       ///< 场景栈，存储当前所有活动场景

    PendingAction m_pendingAction;                          ///< 当前待处理的场景操作类型
    std::unique_ptr<Scene> m_pendingScene;                  ///< 待处理的新场景（用于Push和Replace操作）

public:
    explicit SceneManager(engine::core::Context& context);
    ~SceneManager();

    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;

    Scene* getCurrentScene() const;
    engine::core::Context& getContext() const;

    void update(float deltaTime);
    void render();
    void handleInput();
    void close();

private:
    // 事件回调函数
    void onPopScene();
    void onPushScene(engine::utils::PushSceneEvent& event);
    void onReplaceScene(engine::utils::ReplaceSceneEvent& event);

    void processPendingActions();
    void pushScene(std::unique_ptr<Scene>&& scene);
    void popScene();
    void replaceScene(std::unique_ptr<Scene>&& scene);
};

} // namespace engine::scene
