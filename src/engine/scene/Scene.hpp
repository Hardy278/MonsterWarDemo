#pragma once
#include <vector>
#include <memory>
#include <string>
#include <string_view>

namespace engine::core {
    class Context;
}

namespace engine::ui {
    class UIManager;
}

namespace engine::object {
    class GameObject;
}

namespace engine::scene {
    class SceneManager;

/**
 * @brief 场景基类，负责管理场景中的游戏对象和场景生命周期。
 *
 * 包含一组游戏对象，并提供更新、渲染、处理输入和清理的接口。
 * 派生类应实现具体的场景逻辑。
 */
class Scene {
protected:
    std::string m_sceneName;                            ///< @brief 场景名称
    engine::core::Context& m_context;                    ///< @brief 上下文引用（隐式，构造时传入）
    std::unique_ptr<engine::ui::UIManager> m_UIManager; ///< @brief UI管理器(初始化时自动创建)
    
    bool m_isInitialized = false;                       ///< @brief 场景是否已初始化(非当前场景很可能未被删除，因此需要初始化标志避免重复初始化)
    std::vector<std::unique_ptr<engine::object::GameObject>> m_gameObjects;         ///< @brief 场景中的游戏对象
    std::vector<std::unique_ptr<engine::object::GameObject>> m_pendingAdditions;    ///< @brief 待添加的游戏对象（延时添加）

public:
    /**
     * @brief 构造函数
     * @param name 场景的名称
     * @param context 场景上下文
     */
    Scene(std::string_view name, engine::core::Context& context);

    // 1. 基类必须声明虚析构函数才能让派生类析构函数被正确调用。
    // 2. 析构函数定义必须写在cpp中，不然需要引入GameObject头文件
    virtual ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = delete;
    Scene& operator=(Scene&&) = delete;

    virtual void init();                        ///< @brief 初始化场景。
    virtual void update(float deltaTime);      ///< @brief 更新场景。
    virtual void render();                      ///< @brief 渲染场景。
    virtual void handleInput();                 ///< @brief 处理输入。
    virtual void clean();                       ///< @brief 清理场景。

    virtual void addGameObject(std::unique_ptr<engine::object::GameObject>&& gameObject);
    virtual void safeAddGameObject(std::unique_ptr<engine::object::GameObject>&& gameObject);
    virtual void removeGameObject(engine::object::GameObject* gameObjectPtr);
    virtual void safeRemoveGameObject(engine::object::GameObject* gameObjectPtr);

    /// @brief 获取场景中的游戏对象容器。
    const std::vector<std::unique_ptr<engine::object::GameObject>>& getGameObjects() const { return m_gameObjects; }
    /// @brief 根据名称查找游戏对象（返回找到的第一个对象）。
    engine::object::GameObject* findGameObjectByName(std::string_view name) const;

    void requestPopScene();
    void requestPushScene(std::unique_ptr<engine::scene::Scene>&& scene);
    void requestReplaceScene(std::unique_ptr<engine::scene::Scene>&& scene);
    void quit();

    // getters and setters
    void setName(std::string_view name) { m_sceneName = name; }               ///< @brief 设置场景名称
    std::string_view getName() const { return m_sceneName; }                  ///< @brief 获取场景名称
    void setInitialized(bool initialized) { m_isInitialized = initialized; }    ///< @brief 设置场景是否已初始化
    bool isInitialized() const { return m_isInitialized; }                      ///< @brief 获取场景是否已初始化

    engine::core::Context& getContext() const { return m_context; }                  ///< @brief 获取上下文引用
    std::vector<std::unique_ptr<engine::object::GameObject>>& getGameObjects() { return m_gameObjects; } ///< @brief 获取场景中的游戏对象

protected:
    void processPendingAdditions();     ///< @brief 处理待添加的游戏对象。（每轮更新的最后调用）
};

} // namespace engine::scene