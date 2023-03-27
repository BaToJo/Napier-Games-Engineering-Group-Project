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
static vector<shared_ptr<Entity>> chains;
static shared_ptr<Entity> bastard;

VertexArray line;

void GameScene::Load()
{
	ls::LoadLevelFile("res/levels/pacman.txt", 50.f);

	// Player Setup
	player = MakeEntity();
	player->setPosition(Vector2f(0.f, 0.f));

	// Player Shape Component
	auto shapeComp = player->addComponent<ShapeComponent>();
	sf::Vector2f size = Vector2f(10.f, 30.f);
	shapeComp->setShape<sf::RectangleShape>(size);
	shapeComp->getShape().setFillColor(Color::Magenta);
	shapeComp->getShape().setOrigin(Vector2f(5.f, 15.f));

	for (int i = 0; i < 3; i++)
	{
		// Chain Setup
		auto chain = MakeEntity();
		chain->setPosition(Vector2f(player->getPosition().x, player->getPosition().y + 25.f + i * 20.f));

		// Chain Shape Component
		auto chainShape = chain->addComponent<ShapeComponent>();
		Vector2f size = Vector2f(5.f, 15.f);
		chainShape->setShape<RectangleShape>(size);
		chainShape->getShape().setFillColor(Color::Green);
		chainShape->getShape().setOrigin(size / 2.f);

		// Chain Physics
		auto chainPhysics = chain->addComponent<ActorPhysicsComponent>(true, size);
		chainPhysics->setMass(20.f - (i * 5));
		//chainPhysics->setFriction(.1f);
		chains.push_back(chain);
	}


	// Ball Setup
	wreckingBall = MakeEntity();
	wreckingBall->setPosition(Vector2f(player->getPosition().x, chains[chains.size() - 1]->getPosition().y + 25.f));

	// Ball Shape Component
	auto ballShape = wreckingBall->addComponent<ShapeComponent>();
	float radius = 15.f;
	ballShape->setShape<sf::CircleShape>(radius);
	ballShape->getShape().setFillColor(Color::Red);
	ballShape->getShape().setOrigin(Vector2f(radius, radius));

	// Ball Physics Component
	auto wreckingBallPhysics = wreckingBall->addComponent<ActorPhysicsComponent>(true, radius);
	
	wreckingBallPhysics->setMass(5.f);
	// Player Physics Component
	auto playerPhysics = player->addComponent<PlayerPhysicsComponent>(size, wreckingBall, chains);
	playerPhysics->setMass(200.f);

	bastard = MakeEntity();
	bastard->setPosition(Vector2f(player->getPosition().x + 200.f, player->getPosition().y - 400.f));
	auto testShape = bastard->addComponent<ShapeComponent>();
	Vector2f testSize = Vector2f(50.f, 50.f);
	testShape->setShape<RectangleShape>(testSize);
	testShape->getShape().setFillColor(sf::Color::Yellow);
	testShape->getShape().setOrigin(Vector2f(testSize.x / 2.f, testSize.y / 2.f));

	auto testPhysics = bastard->addComponent<ActorPhysicsComponent>(true, testSize);
	testPhysics->setMass(100.f);
	// Camera setup
	PlayerCamera.setCenter(player->getPosition());
	PlayerCamera.zoom(0.8);
}

void GameScene::Unload()
{
	player.reset();
	wreckingBall.reset();
	bastard.reset();
	for (auto& e : chains)
		e.reset();
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