#include "GameScene.h"
#include "..\lib_tile_level_loader\LevelSystem.h"
#include "..\components\Cmp_Sprite.h"
#include "..\components\Cmp_Actor_Physics.h"
#include "..\components\Cmp_Player_Physics.h"
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void GameScene::Load()
{
	ls::LoadLevelFile("res/levels/pacman.txt", 50.f);

	// Player Setup
	player = MakeEntity();	
	player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]) + Vector2f(25.f, 25.f));

	// Player Shape Component
	auto shapeComp = player->addComponent<ShapeComponent>();
	shapeComp->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
	shapeComp->getShape().setFillColor(Color::Magenta);
	shapeComp->getShape().setOrigin(Vector2f(10.f, 15.f));

	
	// Player Physics Component
	player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));

	// Camera setup
	PlayerCamera.setCenter(player->getPosition());
	PlayerCamera.zoom(0.8);
}

void GameScene::Unload()
{
	player.reset();
	ls::Unload();
	Scene::Unload();
}

void GameScene::Render()
{
	ls::Render(Engine::getWindow());
	Scene::Render();
}

void GameScene::Update(const double& dt)
{
	Vector2f movement = player->getPosition() - PlayerCamera.getCenter();
	PlayerCamera.move(movement * (float)dt * 5.f);
	PlayerCamera.setCenter(player->getPosition());
	Scene::Update(dt);
}