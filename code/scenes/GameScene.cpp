#include "GameScene.h"
#include "..\lib_tile_level_loader\LevelSystem.h"
#include "..\components\Cmp_Sprite.h"
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void GameScene::Load()
{
	ls::LoadLevelFile("res/levels/pacman.txt", 50.f);
	Scene::Unload();
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