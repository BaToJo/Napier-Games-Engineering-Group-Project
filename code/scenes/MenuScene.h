#pragma once 

#include "..\lib_engine\Engine.h"
#include "../InputManager.h"
#include <SFML/Graphics.hpp>
class MenuScene : public Scene
{
private:
	// Text objects
	sf::Text _gameTitle;
	std::vector<sf::Text> _listOfTexts;
	// Fonts
	sf::Font _font;
	sf::Font _buttonFont;

	sf::RectangleShape _buttonBlueprint;
	std::vector<sf::RectangleShape> _listOfButtons;
	//sf::Color rustColor = { 183, 65, 14, 255 };
	sf::Color rustColor = { 188, 55, 48, 255 };
	sf::Color navyColor = { 32, 42, 68 };


	// Animation variables
	const int alphaLowerBound = 150;
	const int alphaUpperBound = 255;
	const float alphaAmplitude = (alphaUpperBound - alphaLowerBound) / 2.f;
	const float alphaOffset = alphaAmplitude + alphaLowerBound;

	const float alphaOffsetFromZero = alphaUpperBound / 2.f;
	sf::Clock menuClock;

	void UpdatePositions();
public:
	void Load() override;

	void Unload() override;

	void Update(const double& dt) override;

	void Render() override;
};