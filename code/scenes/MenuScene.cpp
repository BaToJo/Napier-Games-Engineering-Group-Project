#include "MenuScene.h"
#include "..\lib_tile_level_loader\LevelSystem.h"
#include "../../lib_engine/System_Renderer.h"
#include "../components/Cmp_Sprite.h"
#include <cmath>
#include "..\game.h"

void MenuScene::UpdatePositions()
{
	_gameTitle.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 40.f));

	for (int i = 0; i < _listOfButtons.size(); i++)
	{
		_listOfButtons[i].setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 200.f + (i * (_buttonBlueprint.getSize().y + 50.f))));
		_listOfTexts[i].setPosition(_listOfButtons[i].getPosition());
	}
}

void MenuScene::Load()
{
	Engine::getWindow().setView(Engine::getWindow().getDefaultView());

	menuClock = sf::Clock();



	_buttonBlueprint.setSize(sf::Vector2f(Engine::getWindowSize().x / 6.4f, Engine::getWindowSize().y / 9.6f));
	_buttonBlueprint.setOrigin(sf::Vector2f(_buttonBlueprint.getSize().x / 2.f, _buttonBlueprint.getSize().y / 2.f));
	_buttonBlueprint.setFillColor(navyColor);


	for (int i = 0; i < 3; i++)
	{
		sf::RectangleShape shape;
		shape.setFillColor(_buttonBlueprint.getFillColor());
		shape.setSize(_buttonBlueprint.getSize());
		shape.setOrigin(_buttonBlueprint.getOrigin());
		shape.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 200.f + (i * (_buttonBlueprint.getSize().y + 50.f))));
		_listOfButtons.push_back(shape);
	}

	

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

	}

	UpdatePositions();
}

void MenuScene::Unload()
{
	_listOfButtons.clear();
	_listOfTexts.clear();
	Scene::Unload();
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
				if (_listOfTexts[i].getString() == "PLAY")
				{
					Engine::ChangeScene(&gameScene);
					return;
				}
					
				if (_listOfTexts[i].getString() == "SETTINGS")
				{
					Engine::ChangeScene(&settingsScene);
					return;
				}
				if (_listOfTexts[i].getString() == "EXIT")
				{
					Engine::getWindow().close();
					return;
				}
					
			}
		}
		else
		{
			_listOfTexts[i].setOutlineColor(sf::Color(255, 255, 255, 0));
		}
	}


	Scene::Update(dt);
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

