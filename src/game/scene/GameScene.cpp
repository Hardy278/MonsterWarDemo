#include "GameScene.hpp"
#include "../../engine/input/InputManager.hpp"
#include "../../engine/core/Context.hpp"

#include <entt/core/hashed_string.hpp>
#include <entt/signal/sigh.hpp>
#include <spdlog/spdlog.h>

namespace game::scene {
GameScene::GameScene(engine::core::Context &context, engine::scene::SceneManager &sceneManager)
    : engine::scene::Scene("GameScene", context, sceneManager) {
}

GameScene::~GameScene() {
}

void GameScene::init() {
    auto& inputManager = m_context.getInputManager();
    inputManager.onAction(entt::hashed_string("attack")).connect<&GameScene::onAttack>(this);
    inputManager.onAction(entt::hashed_string("jump"), engine::input::ActionState::RELEASED).connect<&GameScene::onJump>(this);
}

void GameScene::clean() {
    auto& inputManager = m_context.getInputManager();
    inputManager.onAction(entt::hashed_string("attack")).disconnect<&GameScene::onAttack>(this);
    inputManager.onAction(entt::hashed_string("jump"), engine::input::ActionState::RELEASED).disconnect<&GameScene::onJump>(this);
}

void GameScene::onAttack() {
    spdlog::info("Attack!");
}

void GameScene::onJump() {
    spdlog::info("Jump!");
}
}