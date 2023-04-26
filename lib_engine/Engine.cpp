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
	Renderer::Initialise(window);
	Physics::Initialise();
	ChangeScene(scn);
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
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::F5)) && !F5Pressed)
	{
		F5Pressed = true;
		SaveScene();
	}
	else if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::F5)) && F5Pressed)
	{
		F5Pressed = false;
	}
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::F9)) && !F9Pressed)
	{
		F9Pressed = true;
		LoadScene();
	}
	else if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::F9)) && F9Pressed)
	{
		F9Pressed = false;
	}
	ents.Update(dt);
}

void Scene::Render()
{
	ents.Render();
}

void Scene::SaveScene()
{
	while (savedEnts.list.size() > 0)
	{
		savedEnts.list.at(savedEnts.list.size() - 1)->setForDelete();
		savedEnts.list.at(savedEnts.list.size() - 1)->setAlive(false);
		savedEnts.list.pop_back();
	}
	savedEnts.list.clear();
	for (auto& entity : ents.list)
	{
		// Make a copy of the object pointed to by the entity, and give it to savedEnts.
		savedEnts.list.push_back(std::make_shared<Entity>(*entity));
	}
}

void Scene::LoadScene()
{
	ents.list.clear();
	for (auto& savedEntity : savedEnts.list)
	{
		// Make a copy of the object pointed to by the savedEntity, and give it to ents.
		ents.list.push_back(std::make_shared<Entity>(*savedEntity));
	}
}

std::shared_ptr<Entity> Scene::MakeEntity()
{
	auto e = make_shared<Entity>(this);
	ents.list.push_back(e);
	return std::move(e);
}
