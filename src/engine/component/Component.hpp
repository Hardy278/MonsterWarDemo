#pragma once
#include "../core/Context.hpp"

namespace engine::object {
class GameObject;
}

namespace engine::core {
class Context;
}

namespace engine::component {

/**
 * @brief 组件基类，所有游戏组件的父类
 *
 * Component是所有游戏组件的基类，提供了基本的组件功能接口
 * @note 组件需要附加到GameObject上才能发挥作用
 * @note 每个组件都拥有一个指向其拥有者 GameObject 的指针，通过这个指针可以访问GameObject的属性和方法
 * @note 组件系统采用组合模式，允许通过组合不同的组件来创建复杂的游戏对象
 */
class Component {
    friend class engine::object::GameObject;

  protected:
    engine::object::GameObject *m_owner = nullptr; ///< @brief 指向拥有此组件的游戏对象的指针

  public:
    Component() = default;
    virtual ~Component() = default;

    Component(const Component &) = delete;
    Component &operator=(const Component &) = delete;
    Component(Component &&) = delete;
    Component &operator=(Component &&) = delete;

    void setOwner(engine::object::GameObject *owner);
    engine::object::GameObject *getOwner() const;

  protected:
    virtual void init() {}
    virtual void handleInput(engine::core::Context &) {}
    virtual void update(float, engine::core::Context &) {}
    virtual void render(engine::core::Context &) {}
    virtual void clean() {}
};
} // namespace engine::component
