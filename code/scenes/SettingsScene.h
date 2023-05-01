#pragma once
#include "..\lib_engine\Engine.h"
#include "../InputManager.h"
#include <SFML/Graphics.hpp>

class SettingsScene : public Scene
{
private:
	sf::Text _settingsTitle;
	std::vector<std::string> _settingsNames{"FULLSCREEN: ", "VSYNC: ", "BGM: ", "EFFECTS: "};

	sf::Font _font;
	sf::Font _buttonFont;

	sf::Color rustColor = { 188, 55, 48, 255 };
	sf::Color navyColor = { 32, 42, 68 };

	// List of ui elements
	std::vector<std::pair<sf::RectangleShape, sf::Text>> _uiElements;

	// Animation variables
	const int alphaLowerBound = 150;
	const int alphaUpperBound = 255;
	const float alphaAmplitude = (alphaUpperBound - alphaLowerBound) / 2.f;
	const float alphaOffset = alphaAmplitude + alphaLowerBound;

	void UpdatePositions();

	sf::Clock settingsClock;
public:
	void Load() override;

	void Unload() override;

	void Update(const double& dt) override;

	void Render() override;
};