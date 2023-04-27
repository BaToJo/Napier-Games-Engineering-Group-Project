#include "GameScene.h"
#include "..\lib_tile_level_loader\LevelSystem.h"
#include "..\components\Cmp_Sprite.h"
#include "..\components\Cmp_Actor_Physics.h"
#include "..\components\Cmp_Player_Physics.h"
#include <SFML/Graphics.hpp>


#include <Windows.h>
#include <Xinput.h>


#pragma comment(lib, "XInput.lib")
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
VertexArray line; 
InputManager* manager;

bool isRebind = false;
void GameScene::Load()
{
	ls::LoadLevelFile("res/levels/pacman.txt", 50.f);

	
	manager = new InputManager();

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
	auto playerPhysics = player->addComponent<PlayerPhysicsComponent>(size, manager);
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
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	XInputGetState(0, &state);

	if (((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0))
	{
		std::cout << "A is pressed" << std::endl;
	}

	manager->IsControllerConnected();
	//std::cout << "Left trigger pressing " << (float)state.Gamepad.bLeftTrigger / 255 << std::endl;
	//std::cout << "Right trigger pressing " << (float)state.Gamepad.bRightTrigger / 255 << std::endl;
	
	float leftStickXVal = fmaxf(-1, (float)state.Gamepad.sThumbLX / 32767);
	float leftStickYVal = fmaxf(-1, (float)state.Gamepad.sThumbLY / 32767);

	float deadzoneX = 0.4f;
	float deadzoneY = 0.4f;

	float finalLeftX = (abs(leftStickXVal) < deadzoneX ? 0 : leftStickXVal);
	float finalLeftY = (abs(leftStickYVal) < deadzoneY ? 0 : leftStickYVal);
	
	//std::cout << "Final x " << finalLeftX << std::endl;
	//std::cout << "Final y " << finalLeftY << std::endl;


	//for (int i = 0; i < sf::Joystick::getButtonCount(0); i++)
	//{
	//	if (sf::Joystick::isButtonPressed(0, i))
	//		std::cout << "Button pressed, corresponding ID " << i << std::endl;
	//}

	//std::cout << "Position of X axis " << sf::Joystick::getAxisPosition(0, sf::Joystick::X) << std::endl;
	//std::cout << "Position of Y axis " << sf::Joystick::getAxisPosition(0, sf::Joystick::Y) << std::endl;
	//std::cout << "Position of Z axis " << sf::Joystick::getAxisPosition(0, sf::Joystick::Z) << std::endl;
	//std::cout << "Position of R axis " << sf::Joystick::getAxisPosition(0, sf::Joystick::R) << std::endl;


	if (isRebind)
	{
		sf::Keyboard::Key rebind = sf::Keyboard::Unknown;
		for (sf::Keyboard::Key k = (sf::Keyboard::Key)0; k != sf::Keyboard::KeyCount; k = (sf::Keyboard::Key)(k + 1))
		{
			if (sf::Keyboard::isKeyPressed(k) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q) != true)
			{
				rebind = k;
				isRebind = false;
			}
				
		}
		manager->RebindKeyboard(0, rebind);
		
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		isRebind = true;
	Vector2f movement = player->getPosition() - PlayerCamera.getCenter();
	PlayerCamera.move(movement * (float)dt * 5.f);
	PlayerCamera.setCenter(player->getPosition());

	line.append(sf::Vertex(player->getPosition()));
	Scene::Update(dt);
}