#include "GameScene.hpp"
#include "../../engine/input/InputManager.hpp"
#include "../../engine/core/Context.hpp"
#include "../../engine/utils/Events.hpp"

#include <entt/signal/dispatcher.hpp>
#include <entt/core/hashed_string.hpp>
#include <entt/signal/sigh.hpp>
#include <spdlog/spdlog.h>

namespace game::scene {
GameScene::GameScene(engine::core::Context &context)
    : engine::scene::Scene("GameScene", context) {
}

GameScene::~GameScene() {
}

void GameScene::init() {
    static int num = 0;
    sceneNum = num++;
    spdlog::info("GameScene::init() {}", sceneNum);

    auto& inputManager = m_context.getInputManager();
    inputManager.onAction(entt::hashed_string{"jump"}).connect<&GameScene::onReplace>(this);  // j
    inputManager.onAction(entt::hashed_string{"mouse_left"}).connect<&GameScene::onPush>(this); // mouse left
    inputManager.onAction(entt::hashed_string{"mouse_right"}).connect<&GameScene::onPop>(this); // mouse right
    inputManager.onAction(entt::hashed_string{"pause"}).connect<&GameScene::onQuit>(this); // p

    Scene::init();
}

void GameScene::clean() {
    auto& inputManager = m_context.getInputManager();
    inputManager.onAction(entt::hashed_string{"jump"}).disconnect<&GameScene::onReplace>(this);  // j
    inputManager.onAction(entt::hashed_string{"mouse_left"}).disconnect<&GameScene::onPush>(this); // mouse left
    inputManager.onAction(entt::hashed_string{"mouse_right"}).disconnect<&GameScene::onPop>(this); // mouse right
    inputManager.onAction(entt::hashed_string{"pause"}).disconnect<&GameScene::onQuit>(this); // p

    Scene::clean();
}

void GameScene::onReplace() {
    spdlog::info("GameScene::onReplace() {}", sceneNum);
    requestReplaceScene(std::make_unique<game::scene::GameScene>(m_context));
}
void GameScene::onPush() {
    spdlog::info("GameScene::onPush() {}", sceneNum);
    requestPushScene(std::make_unique<game::scene::GameScene>(m_context));
}
void GameScene::onPop() {
    spdlog::info("GameScene::onPop() {}", sceneNum);
    requestPopScene();
}
void GameScene::onQuit() {
    spdlog::info("GameScene::onQuit() {}", sceneNum);
    quit();
}
}