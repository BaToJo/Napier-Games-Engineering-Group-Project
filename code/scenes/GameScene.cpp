#include "GameScene.h"
#include "..\lib_tile_level_loader\LevelSystem.h"
#include "..\components\Cmp_Sprite.h"
#include "..\components\Cmp_Actor_Physics.h"
#include "..\components\Cmp_Player_Physics.h"
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> test_NPC;
VertexArray line;

void GameScene::Load()
{
	//ls::LoadLevelFile_OLD("res/levels/pacman.txt", 50.f);
	ls::LoadLevelFile("res/levels/test_large_embeddedTileset.tmj", this, 50.f);

	// Player Setup
	player = MakeEntity();
	player->setPosition(Vector2f(0.f, 0.f));

	// Player Shape Component
	auto shapeComp = player->addComponent<ShapeComponent>();
	sf::Vector2f size = Vector2f(10.f, 30.f);
	shapeComp->setShape<sf::RectangleShape>(size);
	shapeComp->getShape().setFillColor(Color::Magenta);
	shapeComp->getShape().setOrigin(Vector2f(5.f, 15.f));

	// Player Physics Component
	auto playerPhysics = player->addComponent<PlayerPhysicsComponent>(size);
	// Camera setup
	PlayerCamera.setCenter(player->getPosition());
	PlayerCamera.zoom(0.8);

	// Spawn a green circle on top of every waypoint for debugging purposes.



	/*
	// Make a test NPC
	test_NPC = MakeEntity();
	test_NPC->setPosition(sf::Vector2f(0.f, 0.f));

	// Test NPC Shape Component
	auto shapeCompNPC = test_NPC->addComponent<ShapeComponent>();
	sf::Vector2f sizeNPC = Vector2f(10.f, 30.f);
	shapeCompNPC->setShape<sf::RectangleShape>(size);
	shapeCompNPC->getShape().setFillColor(Color::Red);
	shapeCompNPC->getShape().setOrigin(Vector2f(5.f, 15.f));
	*/
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
	Engine::getWindow().draw(line);
	Scene::Render();
}

void GameScene::Update(const double& dt)
{
	Vector2f movement = player->getPosition() - PlayerCamera.getCenter();
	PlayerCamera.move(movement * (float)dt * 5.f);
	PlayerCamera.setCenter(player->getPosition());

	line.append(sf::Vertex(player->getPosition()));
	Scene::Update(dt);
}