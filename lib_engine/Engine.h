#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <Ecm.h>
#include <maths.h>

class Scene
{
public:
	Scene() = delete;
	virtual ~Scene();
	virtual void Load() = 0;
	virtual void Unload();
	virtual void Update(const double& dt);
	virtual void Render();

	std::shared_ptr<Entity> MakeEntity();

	EntityManager ents;
};
class Engine 
{
public:
	Engine() = delete;

	static void Start(unsigned int width, unsigned int height, const std::string& gameName, Scene* scn);
	static sf::RenderWindow& getWindow();
	static sf::Vector2u getWindowSize();
	static void ChangeScene(Scene*);
	
private:
	static Scene* _activeScene;
	static std::string _gameName;
	static void Update();
	static void Render(sf::RenderWindow& window);
};

