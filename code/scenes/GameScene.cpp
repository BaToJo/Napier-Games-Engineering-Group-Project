#include "GameScene.h"
#include "..\lib_tile_level_loader\LevelSystem.h"
#include "..\components\Cmp_Sprite.h"
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void GameScene::Load()
{
	ls::LoadLevelFile("res/levels/pacman.txt", 50.f);

	player = MakeEntity();
	
	player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]) + Vector2f(25.f, 25.f));


	auto shapeComp = player->addComponent<ShapeComponent>();
	shapeComp->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
	shapeComp->getShape().setFillColor(Color::Magenta);
	shapeComp->getShape().setOrigin(Vector2f(10.f, 15.f));


	this->PlayerCamera.setCenter(player->getPosition());
	this->PlayerCamera.zoom(0.8);
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
	Scene::Update(dt);
}