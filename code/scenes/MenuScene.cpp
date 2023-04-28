#include "MenuScene.h"
#include "..\lib_tile_level_loader\LevelSystem.h"
#include "../../lib_engine/System_Renderer.h"
#include "../components/Cmp_Sprite.h"
#include <cmath>
sf::Text _gameTitle;
sf::Text _gameTitleShadow;
sf::Text _playText;

sf::Font _font;
sf::Font _buttonFont;

sf::RectangleShape buttonTest;

//sf::Color rustColor = { 183, 65, 14, 255 };
sf::Color rustColor = { 188, 55, 48, 255 };
const int alphaLowerBound = 150;
const int alphaUpperBound = 255;
const float alphaAmplitude = (alphaUpperBound - alphaLowerBound) / 2.f;
const float alphaOffset = alphaAmplitude + alphaLowerBound;
sf::Clock menuClock;
void MenuScene::Load()
{
	PlayerCamera.setCenter(sf::Vector2f(Engine::getWindowSize().x / 2.f, Engine::getWindowSize().y / 2.f));
	PlayerCamera.zoom(1.f);

	menuClock = sf::Clock();
	buttonTest.setSize(sf::Vector2f(200.f, 75.f));
	buttonTest.setOrigin(sf::Vector2f(buttonTest.getSize().x / 2.f, buttonTest.getSize().y / 2.f));
	buttonTest.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 200.f));
	buttonTest.setFillColor(sf::Color(32, 42, 68));



	if (_font.loadFromFile("res/fonts/ChakraPetch-Regular.ttf"))
	{
		_gameTitle.setFont(_font);
		_gameTitleShadow.setFont(_font);

		_gameTitle.setString("CARTHARSIS");
		_gameTitleShadow.setString("CARTHARSIS");
		_gameTitle.setCharacterSize(100);
		_gameTitleShadow.setCharacterSize(101);

		_gameTitle.setFillColor(rustColor);
		_gameTitleShadow.setFillColor(sf::Color(rustColor.r, rustColor.g, rustColor.b, 100));

		_gameTitle.setOrigin(_gameTitle.getGlobalBounds().getSize() / 2.f + _gameTitle.getLocalBounds().getPosition());

		_gameTitleShadow.setOrigin(_gameTitleShadow.getGlobalBounds().getSize() / 2.f + _gameTitleShadow.getLocalBounds().getPosition());

		_gameTitle.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 40.f));
		_gameTitleShadow.setPosition(_gameTitle.getPosition());
	}
	

	if (_buttonFont.loadFromFile("res/fonts/BrunoAce-Regular.ttf"))
	{
		_playText.setFont(_buttonFont);
		_playText.setString("PLAY");
		_playText.setCharacterSize(26);
		_playText.setFillColor(rustColor);
		_playText.setOrigin(_playText.getGlobalBounds().getSize() / 2.f + _playText.getLocalBounds().getPosition());
		_playText.setPosition(sf::Vector2f(buttonTest.getPosition()));
	}
}

void MenuScene::Unload()
{
}

void MenuScene::Update(const double& dt)
{
	const float alpha = alphaAmplitude * std::sin(menuClock.getElapsedTime().asSeconds() * 1.f * M_PI) + alphaOffset;
	_gameTitle.setFillColor(sf::Color(rustColor.r, rustColor.g, rustColor.b, alpha));

}

void MenuScene::Render()
{
	//ls::Render(Engine::getWindow());
	Engine::getWindow().draw(_gameTitle);
	Engine::getWindow().draw(_gameTitleShadow);
	Engine::getWindow().draw(buttonTest);
	Engine::getWindow().draw(_playText);
	Scene::Render();
}
