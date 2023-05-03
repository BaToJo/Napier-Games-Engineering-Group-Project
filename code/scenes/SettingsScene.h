#pragma once
#include "..\lib_engine\Engine.h"
#include "../InputManager.h"
#include <SFML/Graphics.hpp>
#include <map>
class SettingsScene : public Scene
{
private:
	sf::Text _settingsTitle;

	std::vector<std::string> _settingsNames{"FULLSCREEN", "VSYNC", "BGM", "EFFECTS"};

	sf::Text _controlsText;
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
	void UpdateKeys();
	void UpdateResolution();
	sf::Clock settingsClock;

	bool isMousePressed = false;
	bool isRebinding = false;
	int indexRebind = -1;
	sf::Keyboard::Key currentKeyPressed = sf::Keyboard::Unknown;
public:
	void Load() override;

	void Unload() override;

	void Update(const double& dt) override;

	void Render() override;

	void HandleEvents() override;

};
