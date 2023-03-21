#include "GameScene.h"
#include "..\lib_tile_level_loader\LevelSystem.h"
#include "..\components\Cmp_Sprite.h"
#include "..\components\Cmp_Actor_Physics.h"
#include "..\components\Cmp_Player_Physics.h"
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> wreckingBall;
VertexArray line;
void GameScene::Load()
{
	ls::LoadLevelFile("res/levels/pacman.txt", 50.f);

	// Player Setup
	player = MakeEntity();	
	player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]) + Vector2f(25.f, 25.f));

	// Player Shape Component
	auto shapeComp = player->addComponent<ShapeComponent>();
	sf::Vector2f size = Vector2f(10.f, 30.f);
	shapeComp->setShape<sf::RectangleShape>(size);
	shapeComp->getShape().setFillColor(Color::Magenta);
	shapeComp->getShape().setOrigin(Vector2f(5.f, 15.f));

	


	// Wrecking ball
	wreckingBall = MakeEntity();
	wreckingBall->setPosition(player->getPosition() + Vector2f(0.f, 35.f));

	shapeComp = wreckingBall->addComponent<ShapeComponent>();
	shapeComp->setShape<sf::CircleShape>(10.f);
	shapeComp->getShape().setFillColor(Color::Blue);
	shapeComp->getShape().setOrigin(Vector2f(10.f, 10.f));
	
	auto wreckingBallPhysics = wreckingBall->addComponent<ActorPhysicsComponent>(true, sf::Vector2f(10.f, 10.f));

	//wreckingBallPhysics->setMass(200.f);

	// Player Physics Component
	auto playerPhysics = player->addComponent<PlayerPhysicsComponent>(size, wreckingBallPhysics.get());
	playerPhysics->setMass(200.f);
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
	Engine::getWindow().draw(line);
	Scene::Render();
}

void GameScene::Update(const double& dt)
{
	Vector2f movement = player->getPosition() - PlayerCamera.getCenter();
	PlayerCamera.move(movement * (float)dt * 5.f);
	PlayerCamera.setCenter(player->getPosition());

	line.append(sf::Vertex(player->getPosition()));
	
	line.append(sf::Vertex(wreckingBall->getPosition()));
	Scene::Update(dt);
}