#include "GameScene.h"
#include "..\lib_tile_level_loader\LevelSystem.h"
#include "..\components\Cmp_Sprite.h"
#include "..\components\Cmp_Actor_Physics.h"
#include "..\components\Cmp_Player_Physics.h"
#include <SFML/Graphics.hpp>


#include <Windows.h>
#include <Xinput.h>


#pragma comment(lib, "XInput.lib")
#include "../lib_engine/Audio.h"
#include "../game.h"
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> wreckingBall;
static vector<shared_ptr<Entity>> chains;
static shared_ptr<Entity> cube;

VertexArray line;

bool isRebind = false;
bool hasBeenLoaded = false;
void GameScene::Load()
{
	if (!hasBeenLoaded)
	{
		PlayerCamera.zoom(0.8);
		hasBeenLoaded = true;
	}
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

	for (int i = 0; i < 2; i++)
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
	playerPhysics->setMass(20.f);

	cube = MakeEntity();
	cube->setPosition(Vector2f(player->getPosition().x + 200.f, player->getPosition().y - 400.f));
	auto testShape = cube->addComponent<ShapeComponent>();
	Vector2f testSize = Vector2f(50.f, 50.f);
	testShape->setShape<RectangleShape>(testSize);
	testShape->getShape().setFillColor(sf::Color::Yellow);
	testShape->getShape().setOrigin(Vector2f(testSize.x / 2.f, testSize.y / 2.f));

	auto testPhysics = cube->addComponent<ActorPhysicsComponent>(true, testSize);
	testPhysics->setMass(100.f);
	// Camera setup
	PlayerCamera.setCenter(player->getPosition());


	if (_settings[2].second)
	{
		Audio::Music_Load_from_file("res/audio/music_ambience_city_2.ogg", "ambience_city_2");
		Audio::Music_Play("ambience_city_2", 0.4, 1.0);
	}


	if (_settings[3].second)
	{
		Audio::Sound_Load_from_file("res/audio/sound_engine_rev_loop.ogg", "engine_rev");
		Audio::Sound_Load_from_file("res/audio/sound_engine_idle_fastest_loop.ogg", "engine_idle");
		Audio::Sound_Play_Looping("engine_rev", 0.0f, 1.0f);
		Audio::Sound_Play_Looping("engine_idle", 0.0f, 1.0f);
	}


	Engine::getWindow().setView(PlayerCamera);
}

void GameScene::Unload()
{
	player.reset();
	wreckingBall.reset();
	cube.reset();
	for (auto& e : chains)
		e.reset();
	chains.clear();

	Audio::UnloadAll();
	ls::Unload();
	Scene::Unload();
}

void GameScene::Render()
{
	ls::Render(Engine::getWindow());
	Engine::getWindow().draw(line);
	Scene::Render();
}

void GameScene::HandleEvents()
{
	Scene::HandleEvents();
}

void GameScene::Update(const double& dt)
{
	Engine::getWindow().setView(PlayerCamera);

	//if (isRebind)
	//{
	//	sf::Keyboard::Key rebind = sf::Keyboard::Unknown;
	//	for (sf::Keyboard::Key k = (sf::Keyboard::Key)0; k != sf::Keyboard::KeyCount; k = (sf::Keyboard::Key)(k + 1))
	//	{
	//		if (sf::Keyboard::isKeyPressed(k) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q) != true)
	//		{
	//			rebind = k;
	//			isRebind = false;
	//		}

	//	}
	//	_inputManager.RebindKeyboard(0, rebind);

	//}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		isRebind = true;
	Vector2f movement = player->getPosition() - PlayerCamera.getCenter();
	PlayerCamera.move(movement * (float)dt * 5.f);
	PlayerCamera.setCenter(player->getPosition());

	line.append(sf::Vertex(player->getPosition()));
	Scene::Update(dt);
}