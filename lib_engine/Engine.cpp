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
auto currentWindowStyle = sf::Style::Default;
Vector2u persistentWindowSize = Vector2u(800, 600);

void Engine::Update(double dt)
{
	//float dt = clock.restart().asSeconds();

	Physics::Update(dt);
	_activeScene->Update(dt);
}

void Engine::Render(RenderWindow& window)
{
	_activeScene->Render();
	Renderer::Render();
}

void Engine::WindowResize(float x, float y)
{
	sf::FloatRect visibleArea(Vector2f(0, 0), Vector2f(x, y));
	_activeScene->PlayerCamera = sf::View(visibleArea);
}

void Engine::Start(unsigned int width, unsigned int height, const std::string& gameName, Scene* scn)
{

	RenderWindow window(VideoMode(Vector2u(width, height)), gameName, currentWindowStyle);
	// window.setFramerateLimit(60.0f);
	_gameName = gameName;
	_window = &window;
	Renderer::Initialise(window);
	Physics::Initialise();
	ChangeScene(scn);
	if (currentWindowStyle = sf::Style::Default)
		WindowResize(window.getSize().x, window.getSize().y);
	else
		WindowResize(sf::VideoMode::getDesktopMode().size.y, sf::VideoMode::getDesktopMode().size.y);

	

	static sf::Clock clock;

	double t = 0;
	double dt = 1/60.f;

	double currentTime = clock.getElapsedTime().asSeconds();
	double accumulator = 0.0f;

	bool hotkeyDown_AltEnter = false;

	while (window.isOpen())
	{

		double newTime = clock.getElapsedTime().asSeconds();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
			if (event.type == Event::Resized)
			{
				WindowResize(event.size.width, event.size.height);
			}
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				window.close();
			}

			if (event.type == Event::MouseWheelScrolled)
			{
				float zoomDelta = 1 + (event.mouseWheelScroll.delta * -0.1);
				_activeScene->PlayerCamera.zoom(zoomDelta);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			{
				if (!hotkeyDown_AltEnter)
				{
					hotkeyDown_AltEnter = true;
					if (currentWindowStyle == sf::Style::Default)
					{
						persistentWindowSize = Vector2u(window.getSize());
						currentWindowStyle = sf::Style::Fullscreen;
					}
					else
					{
						currentWindowStyle = sf::Style::Default;
					}
					window.create(VideoMode(persistentWindowSize), gameName, currentWindowStyle);
					if (currentWindowStyle == sf::Style::Default)
						WindowResize(window.getSize().x, window.getSize().y);
					else
						WindowResize(sf::VideoMode::getDesktopMode().size.y, sf::VideoMode::getDesktopMode().size.y);
				}
			}
			else
			{
				hotkeyDown_AltEnter = false;
			}



		}

		window.clear();
		while (accumulator >= dt)
		{
			Update(dt);
			accumulator -= dt;
			t += dt;
		}
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
