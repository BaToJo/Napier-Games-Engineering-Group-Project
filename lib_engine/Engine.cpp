#include "Engine.h"
#include "maths.h"
#include "System_Physics.h"
#include "System_Renderer.h"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

Scene* Engine::_activeScene = nullptr;
std::string Engine::_gameName;
static RenderWindow* _window;

void Engine::Update()
{
	static sf::Clock clock;
	float dt = clock.restart().asSeconds();

	Physics::Update(dt);
	_activeScene->Update(dt);
}

void Engine::Render(RenderWindow& window)
{
	_activeScene->Render();
	Renderer::Render();
}

void Engine::Start(unsigned int width, unsigned int height, const std::string& gameName, Scene* scn)
{
	RenderWindow window(VideoMode(Vector2u(width, height)), gameName);
	_gameName = gameName;
	_window = &window;


	ChangeScene(scn);
	Renderer::Initialise(window);
	Physics::Initialise();

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				window.close();
			}
		}

		window.clear();
		Update();
		window.setView(scn->PlayerCamera);
		Render(window);
		window.display();

	}

	if (_activeScene != nullptr)
	{
		_activeScene->Unload();
		_activeScene = nullptr;
	}

	window.close();
	Physics::Shutdown();
}

sf::RenderWindow& Engine::getWindow()
{
	return *_window;
}

sf::Vector2u Engine::getWindowSize()
{
	return _window->getSize();
}

void Engine::ChangeScene(Scene* s)
{
	auto old = _activeScene;
	_activeScene = s;

	if (old != nullptr)
	{
		old->Unload();
	}

	_activeScene->Load();
}



Scene::~Scene()
{
	Unload();
}

void Scene::Unload()
{
	ents.list.clear();
}

void Scene::Update(const double& dt)
{
	ents.Update(dt);
}

void Scene::Render()
{
	ents.Render();
}

std::shared_ptr<Entity> Scene::MakeEntity()
{
	auto e = make_shared<Entity>(this);
	ents.list.push_back(e);
	return std::move(e);
}
