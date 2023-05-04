#pragma once 

#include "..\lib_engine\Engine.h"
#include "../InputManager.h"
#include <SFML/Graphics.hpp>
class GameScene : public Scene
{
private:
	sf::Text _score;
	int _scoreNumber = 0;
	sf::Font _font;
	sf::Color rustColor = { 188, 55, 48, 255 };

public:
	void Load() override;

	void Unload() override;

	void Update(const double& dt) override;

	void Render() override;

	void HandleEvents() override;

};