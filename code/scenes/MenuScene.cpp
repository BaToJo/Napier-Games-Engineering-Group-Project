#include "MenuScene.h"
#include "..\lib_tile_level_loader\LevelSystem.h"
#include "../../lib_engine/System_Renderer.h"
#include "../components/Cmp_Sprite.h"
#include <cmath>
#include "..\game.h"

// Text objects
sf::Text _gameTitle;

sf::Text _playText;
sf::Text _settingsText;
sf::Text _exitText;

std::vector<sf::Text> _listOfTexts;
// Fonts
sf::Font _font;
sf::Font _buttonFont;
std::vector<sf::Font> _listOfFonts;


sf::RectangleShape _buttonBlueprint;

sf::RectangleShape _playButton;
sf::RectangleShape _settingsButton;
sf::RectangleShape _exitButton;

std::vector<sf::RectangleShape> _listOfButtons;
//sf::Color rustColor = { 183, 65, 14, 255 };
sf::Color rustColor = { 188, 55, 48, 255 };


// Animation variables
const int alphaLowerBound = 150;
const int alphaUpperBound = 255;
const float alphaAmplitude = (alphaUpperBound - alphaLowerBound) / 2.f;
const float alphaOffset = alphaAmplitude + alphaLowerBound;

const float alphaOffsetFromZero = alphaUpperBound / 2.f;
sf::Clock menuClock;



void MenuScene::Load()
{
	Engine::getWindow().setView(Engine::getWindow().getDefaultView());

	menuClock = sf::Clock();



	_buttonBlueprint.setSize(sf::Vector2f(Engine::getWindowSize().x / 6.4f, Engine::getWindowSize().y / 9.6f));
	_buttonBlueprint.setOrigin(sf::Vector2f(_buttonBlueprint.getSize().x / 2.f, _buttonBlueprint.getSize().y / 2.f));
	_buttonBlueprint.setFillColor(sf::Color(32, 42, 68));


	for (int i = 0; i < 3; i++)
	{
		sf::RectangleShape shape;
		shape.setFillColor(_buttonBlueprint.getFillColor());
		shape.setSize(_buttonBlueprint.getSize());
		shape.setOrigin(_buttonBlueprint.getOrigin());
		shape.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 200.f + (i * (_buttonBlueprint.getSize().y + 50.f))));
		_listOfButtons.push_back(shape);
	}

	/*_playButton.setFillColor(_buttonBlueprint.getFillColor());
	_playButton.setSize(_buttonBlueprint.getSize());
	_playButton.setOrigin(_buttonBlueprint.getOrigin());
	_playButton.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 200.f));
	
	_settingsButton.setFillColor(_buttonBlueprint.getFillColor());
	_settingsButton.setSize(_buttonBlueprint.getSize());
	_settingsButton.setOrigin(_buttonBlueprint.getOrigin());
	_settingsButton.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 
		_playButton.getPosition().y + (_playButton.getSize().y / 2) + 50.f));

	_exitButton.setFillColor(_buttonBlueprint.getFillColor());
	_exitButton.setSize(_buttonBlueprint.getSize());
	_exitButton.setOrigin(_buttonBlueprint.getOrigin());
	_exitButton.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f,
		_settingsButton.getPosition().y + (_settingsButton.getSize().y / 2) + 50.f));*/

	if (_font.loadFromFile("res/fonts/ChakraPetch-Regular.ttf"))
	{
		_gameTitle.setFont(_font);

		_gameTitle.setString("CARTHARSIS");
		_gameTitle.setCharacterSize(100);
		_gameTitle.setOutlineColor(sf::Color(rustColor.r, rustColor.g, rustColor.b, 100));
		_gameTitle.setOutlineThickness(1.2f);

		_gameTitle.setFillColor(rustColor);


		_gameTitle.setOrigin(_gameTitle.getGlobalBounds().getSize() / 2.f + _gameTitle.getLocalBounds().getPosition());

		_gameTitle.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 40.f));
	}
	

	if (_buttonFont.loadFromFile("res/fonts/BrunoAce-Regular.ttf"))
	{
		for (int i = 0; i < 3; i++)
		{
			sf::Text buttonText;
			buttonText.setFont(_buttonFont);

			buttonText.setCharacterSize(26);
			buttonText.setOutlineColor(sf::Color(255, 255, 255, 0));
			buttonText.setOutlineThickness(1.2f);

			buttonText.setFillColor(rustColor);

			buttonText.setPosition(sf::Vector2f(_listOfButtons[i].getPosition()));
			_listOfTexts.push_back(buttonText);
		}
		
		_listOfTexts[0].setString("PLAY");
		_listOfTexts[1].setString("SETTINGS");
		_listOfTexts[2].setString("EXIT");

		for (auto& t : _listOfTexts)
			t.setOrigin(t.getGlobalBounds().getSize() / 2.f + t.getLocalBounds().getPosition());

		//_playText.setFont(_buttonFont);

		//_settingsText.setFont(_buttonFont);
		//_exitText.setFont(_buttonFont);

		//_playText.setString("PLAY");

		//_settingsText.setString("SETTINGS");
		//_exitText.setString("EXIT");

		//_playText.setCharacterSize(26);
		//_playText.setOutlineColor(sf::Color(255, 255, 255, 0));
		//_playText.setOutlineThickness(1.2f);

		//_settingsText.setCharacterSize(26);
		//_exitText.setCharacterSize(26);

		//_playText.setFillColor(rustColor);

		//_settingsText.setFillColor(rustColor);
		//_exitText.setFillColor(rustColor);

		//_playText.setOrigin(_playText.getGlobalBounds().getSize() / 2.f + _playText.getLocalBounds().getPosition());


		//_settingsText.setOrigin(_settingsText.getGlobalBounds().getSize() / 2.f + _settingsText.getLocalBounds().getPosition());
		//_exitText.setOrigin(_exitText.getGlobalBounds().getSize() / 2.f + _exitText.getLocalBounds().getPosition());

		//_playText.setPosition(sf::Vector2f(_playButton.getPosition()));

		//_settingsText.setPosition(sf::Vector2f(_settingsButton.getPosition()));
		//_exitText.setPosition(sf::Vector2f(_exitButton.getPosition()));
	}
}

void MenuScene::Unload()
{
}

void MenuScene::Update(const double& dt)
{
	const float alpha = alphaAmplitude * std::sin(menuClock.getElapsedTime().asSeconds() * 1.f * M_PI) + alphaOffset;
	_gameTitle.setFillColor(sf::Color(rustColor.r, rustColor.g, rustColor.b, alpha));


	for (int i = 0; i < _listOfButtons.size(); i++)
	{
		
		if (_listOfButtons[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(Engine::getWindow()))))
		{
			const float outlineAlpha = alphaOffsetFromZero * std::sin(menuClock.getElapsedTime().asSeconds() * 1.f * M_PI) + alphaOffsetFromZero;
			_listOfTexts[i].setOutlineColor(sf::Color(255, 255, 255, outlineAlpha));

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				Engine::ChangeScene(&gameScene);
			}
		}
		else
		{
			_listOfTexts[i].setOutlineColor(sf::Color(255, 255, 255, 0));
		}
	}

}

void MenuScene::Render()
{
	//ls::Render(Engine::getWindow());

	Engine::getWindow().draw(_gameTitle);

	for (auto& b : _listOfButtons)
		Engine::getWindow().draw(b);

	for (auto& t : _listOfTexts)
		Engine::getWindow().draw(t);

	Scene::Render();
}

