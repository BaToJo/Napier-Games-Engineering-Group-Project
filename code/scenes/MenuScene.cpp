#include "MenuScene.h"
#include "..\lib_tile_level_loader\LevelSystem.h"
#include "../../lib_engine/System_Renderer.h"
#include "../components/Cmp_Sprite.h"
#include <cmath>
#include "..\game.h"

// Updating the positions of every UI element to keep everything centered
void MenuScene::UpdatePositions()
{
	_gameTitle.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 80.f));

	for (int i = 0; i < _uiElements.size(); i++)
	{
		_uiElements[i].first.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 240.f + (i * (_uiElements[i].first.getSize().y + 50.f))));
		_uiElements[i].second.setPosition(_uiElements[i].first.getPosition());
	}

}

void MenuScene::Load()
{
	// Setting view to default
	Engine::getWindow().setView(Engine::getWindow().getDefaultView());
	menuClock = sf::Clock();


	// Blueprint for every button in the scene
	sf::RectangleShape buttonBlueprint;
	buttonBlueprint.setSize(sf::Vector2f(Engine::getWindowSize().x / 6.4f, Engine::getWindowSize().y / 9.6f));
	buttonBlueprint.setOrigin(sf::Vector2f(buttonBlueprint.getSize().x / 2.f, buttonBlueprint.getSize().y / 2.f));
	buttonBlueprint.setFillColor(navyColor);

	// Title text
	if (_font.loadFromFile("res/fonts/ChakraPetch-Regular.ttf"))
	{
		_gameTitle.setFont(_font);

		_gameTitle.setString("CARTHARSIS");
		_gameTitle.setCharacterSize(100);
		_gameTitle.setOutlineColor(sf::Color(rustColor.r, rustColor.g, rustColor.b, 100));
		_gameTitle.setOutlineThickness(1.2f);

		_gameTitle.setFillColor(rustColor);


		_gameTitle.setOrigin(_gameTitle.getGlobalBounds().getSize() / 2.f + _gameTitle.getLocalBounds().getPosition());

		_gameTitle.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 80.f));
	}


	if (_buttonFont.loadFromFile("res/fonts/BrunoAce-Regular.ttf"))
	{
		// Setting the color and size of every button
		sf::RectangleShape shape;
		shape.setFillColor(buttonBlueprint.getFillColor());
		shape.setSize(buttonBlueprint.getSize());
		shape.setOrigin(buttonBlueprint.getOrigin());
		

		// Setting the color and the outline of every text
		sf::Text buttonText;
		buttonText.setFont(_buttonFont);

		buttonText.setCharacterSize(26);
		buttonText.setOutlineColor(sf::Color(255, 255, 255, 0));
		buttonText.setOutlineThickness(1.2f);

		buttonText.setFillColor(rustColor);

		// Initialising both text and buttons
		for (int i = 0; i < 3; i++)
		{
			shape.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 240.f + (i * (buttonBlueprint.getSize().y + 50.f))));

			buttonText.setPosition(sf::Vector2f(shape.getPosition()));
			buttonText.setString(_buttonNames[i]);
			_uiElements.push_back(std::pair<sf::RectangleShape, sf::Text>(shape, buttonText));
		}

		// Setting the origin of every text so that it will be centered
		for (auto& pair : _uiElements)
			pair.second.setOrigin(pair.second.getGlobalBounds().getSize() / 2.f + pair.second.getLocalBounds().getPosition());

	}

	UpdatePositions();
}

void MenuScene::Unload()
{
	_uiElements.clear();
	Scene::Unload();
}

void MenuScene::Update(const double& dt)
{
	// This snippet of code will simply make the title pulse
	const float alpha = alphaAmplitude * std::sin(menuClock.getElapsedTime().asSeconds() * 1.f * M_PI) + alphaOffset;
	_gameTitle.setFillColor(sf::Color(rustColor.r, rustColor.g, rustColor.b, alpha));

	for (auto& pair : _uiElements)
	{
		// Check if the mouse is on a button
		if (pair.first.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(Engine::getWindow()))))
		{
			// If it is, start the animation
			const float outlineAlpha = alphaOffsetFromZero * std::sin(menuClock.getElapsedTime().asSeconds() * 1.f * M_PI) + alphaOffsetFromZero;
			pair.second.setOutlineColor(sf::Color(255, 255, 255, outlineAlpha));
			
			// Change scene depending on the button
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (pair.second.getString() == "PLAY")
				{
					Engine::ChangeScene(&gameScene);
					return;
				}

				if (pair.second.getString() == "SETTINGS")
				{
					Engine::ChangeScene(&settingsScene);
					return;
				}
				if (pair.second.getString() == "EXIT")
				{
					Engine::getWindow().close();
					return;
				}
			}
		}
		else
		{
			// If nothing is selected, just set the outline to white
			pair.second.setOutlineColor(sf::Color(255, 255, 255, 0));
		}
	}

	UpdatePositions();
	Scene::Update(dt);
}

void MenuScene::Render()
{

	Engine::getWindow().draw(_gameTitle);

	for (auto& pair : _uiElements)
	{
		Engine::getWindow().draw(pair.first);
		Engine::getWindow().draw(pair.second);
	}


	Scene::Render();
}

