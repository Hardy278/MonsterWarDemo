/**
 * @file Camera.hpp
 * @brief 相机系统头文件
 */

#pragma once
#include "../utils/Math.hpp"
#include <optional>

namespace engine::component {
class TransformComponent;
}

namespace engine::render {
/**
 * @class Camera
 * @brief 2D相机类，用于处理游戏世界的视图转换和相机移动
 *
 * 该类实现了2D游戏中的相机系统，包括：
 * @brief - 世界坐标到屏幕坐标的转换
 * @brief - 视差滚动支持
 * @brief - 相机移动和边界限制
 * @brief - 视口管理
 */
class Camera final {
  private:
    glm::vec2 m_viewportSize;                          ///< 视口大小
    glm::vec2 m_position;                              ///< 相机在世界坐标中的位置
    std::optional<engine::utils::Rect> m_limitBounds;  ///< 相机移动的边界限制
    float m_smoothSpeed = 3.0f;                        ///< @brief 相机移动的平滑速度
    component::TransformComponent *m_target = nullptr; ///< @brief 跟随目标变换组件，空值表示不跟随

  public:
    /**
     * @brief 构造函数
     * @param viewportSize 视口大小
     * @param position 初始位置，默认为(0,0)
     * @param limitBounds 可选的相机移动边界限制
     */
    Camera(const glm::vec2 &viewportSize, const glm::vec2 &position = {0.0f, 0.0f}, const std::optional<engine::utils::Rect> &limitBounds = std::nullopt);

    Camera(const Camera &) = delete;
    Camera &operator=(const Camera &) = delete;
    Camera(Camera &&) = delete;
    Camera &operator=(Camera &&) = delete;

    void update(float deltaTime);
    void move(const glm::vec2 &offset);

    glm::vec2 worldToScreen(const glm::vec2 &worldPos) const;
    glm::vec2 worldToScreenWithParallax(const glm::vec2 &worldPos, const glm::vec2 &scrollFactor) const;
    glm::vec2 screenToWorld(const glm::vec2 &screenPos) const;

    void setPosition(const glm::vec2 &position);
    void setLimitBounds(std::optional<engine::utils::Rect> limitBounds);
    void setTarget(engine::component::TransformComponent *target);

    const glm::vec2 &getPosition() const;
    const std::optional<engine::utils::Rect> getLimitBounds() const;
    const glm::vec2 getViewportSize() const;
    engine::component::TransformComponent *getTarget() const;

  private:
    void clampPosition();
};
} // namespace engine::render
