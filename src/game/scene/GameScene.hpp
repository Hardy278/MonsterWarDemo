#pragma once
#include "../../engine/scene/Scene.hpp"

namespace game::scene {
class GameScene final : public engine::scene::Scene {
public:
    GameScene(engine::core::Context& context, engine::scene::SceneManager& sceneManager);
    ~GameScene();

    void init() override;
    void clean() override;
private:
    void onAttack();
    void onJump();
};
}