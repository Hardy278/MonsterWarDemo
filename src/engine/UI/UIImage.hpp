#pragma once
#include "../render/Sprite.hpp"
#include "UIElement.hpp"

#include <SDL3/SDL_rect.h>

#include <optional>
#include <string>
#include <string_view>

namespace engine::ui {

/**
 * @brief 一个用于显示纹理或部分纹理的UI元素。
 *
 * 继承自UIElement并添加了渲染图像的功能。
 */
class UIImage final : public UIElement {
  protected:
    engine::render::Sprite m_sprite;

  public:
    /**
     * @brief 构造一个UIImage对象。
     * @param textureID 要显示的纹理ID。
     * @param position 图像的局部位置。
     * @param size 图像元素的大小。（如果为{0,0}，则使用纹理的原始尺寸）
     * @param sourceRect 可选：要绘制的纹理部分。（如果为空，则使用纹理的整个区域）
     * @param isFlipped 可选：精灵是否应该水平翻转。
     */
    UIImage(
        std::string_view textureID,
        glm::vec2 position = {0.0f, 0.0f},
        glm::vec2 size = {0.0f, 0.0f},
        std::optional<SDL_FRect> sourceRect = std::nullopt,
        bool isFlipped = false);

    void render(engine::core::Context &context) override;
    
    const engine::render::Sprite &getSprite() const { return m_sprite; }
    std::string_view getTextureID() const { return m_sprite.getTextureID(); }
    const std::optional<SDL_FRect> &getSourceRect() const { return m_sprite.getSourceRect(); }

    bool isFlipped() const { return m_sprite.isFlipped(); }
    void setFlipped(bool flipped) { m_sprite.setFlipped(flipped); }
    void setSprite(engine::render::Sprite sprite) { m_sprite = std::move(sprite); }
    void setTextureID(std::string_view textureID) { m_sprite.setTextureID(textureID); }
    void setSourceRect(std::optional<SDL_FRect> sourceRect) { m_sprite.setSourceRect(std::move(sourceRect)); }
};

} // namespace engine::ui