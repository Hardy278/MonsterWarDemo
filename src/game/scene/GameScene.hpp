#pragma once
#include "../../engine/scene/Scene.hpp"

namespace game::scene {
class GameScene final : public engine::scene::Scene {
  public:
    GameScene(engine::core::Context &context);
    ~GameScene();

    void init() override;
    void clean() override;

  private:
    int sceneNum{0};
    void onReplace();
    void onPush();
    void onPop();
    void onQuit();
};
} // namespace game::scene