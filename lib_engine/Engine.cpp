#include "Engine.h"
#include "maths.h"
#include "System_Physics.h"
#include "System_Renderer.h"
#include <SFML/Graphics.hpp>
#include "../code/scenes/GameScene.h"
#include "../code/game.h"

using namespace sf;
using namespace std;

Scene* Engine::_activeScene = nullptr;
std::string Engine::_gameName;
static RenderWindow* _window;

void Engine::Update(double dt)
{
	//float dt = clock.restart().asSeconds();
	//std::cout << "Dt " << dt << std::endl;
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
	//window.setFramerateLimit(60);
	//window.setKeyRepeatEnabled(false);
	_gameName = gameName;
	_window = &window;
	Renderer::Initialise(window);
	Physics::Initialise();
	ChangeScene(scn);
	

	static sf::Clock clock;

	double t = 0;
	double dt = 0.01;

	double currentTime = clock.getElapsedTime().asSeconds();
	double accumulator = 0.0f;

	while (window.isOpen())
	{
		
		double newTime = clock.getElapsedTime().asSeconds();
		double frameTime = newTime - currentTime;
		if (frameTime > 0.25)
			frameTime = 0.25;
		currentTime = newTime;

		accumulator += frameTime;

		_activeScene->HandleEvents();
		window.clear();

		while (accumulator >= dt)
		{
			Update(dt);
			t += dt;
			accumulator -= dt;
		}
		const double alpha = accumulator / dt;
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

void Scene::HandleEvents()
{
	Event event;
	while (Engine::getWindow().pollEvent(event))
	{
		if (event.type == Event::Closed)
		{
			Engine::getWindow().close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			Engine::ChangeScene(&menuScene);
		}
		if (event.type == Event::Resized)
		{
			sf::FloatRect visibleArea(sf::Vector2f(0.f, 0.f), sf::Vector2f(event.size.width, event.size.height));
			Engine::getWindow().setView(sf::View(visibleArea));
		}
	}

}

std::shared_ptr<Entity> Scene::MakeEntity()
{
	auto e = make_shared<Entity>(this);
	ents.list.push_back(e);
	return std::move(e);
}
