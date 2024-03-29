#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
//#include <Ecm.h>
#include "../lib_ecm/Ecm.h"
#include "..\lib_maths\maths.h"

class Scene
{
public:
	Scene() = default;
	virtual ~Scene();
	virtual void Load() = 0;
	virtual void Unload();
	virtual void Update(const double& dt);
	virtual void Render();
	virtual void HandleEvents();
	std::shared_ptr<Entity> MakeEntity();

	EntityManager ents;
	sf::View PlayerCamera;

};

class Engine
{
public:
	Engine() = delete;

	static void Start(unsigned int width, unsigned int height, const std::string& gameName, Scene* scn, bool fullscreen);
	static sf::RenderWindow& getWindow();
	static sf::Vector2u getWindowSize();
	static void ChangeScene(Scene*);
	static Scene* GetActiveScene();
	static void WindowResize(float x, float y);
	static Scene* _activeScene;

private:
	static std::string _gameName;
	static void Update(double dt);
	static void Render(sf::RenderWindow& window);
};
