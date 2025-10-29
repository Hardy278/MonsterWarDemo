#include "engine/core/Game.hpp"
#include "engine/core/Context.hpp"
#include "engine/scene/SceneManager.hpp"
#include "engine/utils/Events.hpp"
#include "game/scene/GameScene.hpp"

#include <entt/signal/dispatcher.hpp>
#include <spdlog/spdlog.h>
#include <SDL3/SDL.h>

void setupInitialScene(engine::core::Context& context) {
    // GameApp在调用run方法之前，先创建并设置初始场景
    auto titleScene = std::make_unique<game::scene::GameScene>(context);
    context.getDispatcher().trigger<engine::utils::PushSceneEvent>(engine::utils::PushSceneEvent{std::move(titleScene)});
}

int main() {
    spdlog::set_level(spdlog::level::trace);

    engine::core::Game app;
    app.registerSceneSetup(setupInitialScene);
    app.run();
    return 0;
}