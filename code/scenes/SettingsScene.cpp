#include "SettingsScene.h"

#include "../game.h"


void SettingsScene::UpdatePositions()
{
	_settingsTitle.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 40.f));

	for (int i = 0; i < _uiElements.size(); i++)
	{
		_uiElements[i].first.setPosition(sf::Vector2f(
			(Engine::getWindowSize().x / 2.f) + _settingsTitle.getGlobalBounds().width / 2.f,
			_uiElements[i].second.getPosition().y + _uiElements[i].second.getGlobalBounds().height / 2.f + _uiElements[i].first.getSize().y / 4.f)
		);

		_uiElements[i].second.setPosition(sf::Vector2f(
			(Engine::getWindowSize().x / 2.f) - _settingsTitle.getGlobalBounds().width,
			200.f + (i * (_uiElements[i].second.getGlobalBounds().height + 50.f)
		)));
	}

}

void SettingsScene::Load()
{
	Engine::getWindow().setView(Engine::getWindow().getDefaultView());

	settingsClock = sf::Clock();

	if (_font.loadFromFile("res/fonts/ChakraPetch-Regular.ttf"))
	{
		_settingsTitle.setFont(_font);

		_settingsTitle.setString("SETTINGS");
		_settingsTitle.setCharacterSize(100);
		_settingsTitle.setOutlineColor(sf::Color(rustColor.r, rustColor.g, rustColor.b, 100));
		_settingsTitle.setOutlineThickness(1.2f);

		_settingsTitle.setFillColor(rustColor);


		_settingsTitle.setOrigin(_settingsTitle.getGlobalBounds().getSize() / 2.f + _settingsTitle.getLocalBounds().getPosition());

		_settingsTitle.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 40.f));
	}


	if (_buttonFont.loadFromFile("res/fonts/BrunoAce-Regular.ttf"))
	{
		for (int i = 0; i < 4; i++)
		{
			sf::Text buttonText;
			buttonText.setFont(_buttonFont);
			buttonText.setCharacterSize(30);
			buttonText.setOutlineColor(sf::Color(255, 255, 255, 0));
			buttonText.setOutlineThickness(1.2f);

			
			buttonText.setString(_settingsNames[i]);
			buttonText.setFillColor(rustColor);

			buttonText.setPosition(sf::Vector2f(
				(Engine::getWindowSize().x / 2.f) - _settingsTitle.getGlobalBounds().width,
				100.f + (i * (buttonText.getGlobalBounds().height + 50.f)
					)));

			sf::RectangleShape rect;
			rect.setSize(sf::Vector2f(20.f, 20.f));
			rect.setOrigin(sf::Vector2f(rect.getSize().x / 2.f, rect.getSize().y / 2.f));
			rect.setOutlineColor(sf::Color::White);
			rect.setOutlineThickness(1.2f);
			rect.setPosition(sf::Vector2f(
				(Engine::getWindowSize().x / 2.f) + _settingsTitle.getGlobalBounds().width / 2.f,
				buttonText.getPosition().y + buttonText.getGlobalBounds().height / 2.f + rect.getSize().y / 4.f)
			);

			_uiElements.push_back(std::pair<sf::RectangleShape, sf::Text>(rect, buttonText));

		}

	}


}

void SettingsScene::Unload()
{
	_uiElements.clear();
	Scene::Unload();
}

void SettingsScene::Update(const double& dt)
{
	const float alpha = alphaAmplitude * std::sin(settingsClock.getElapsedTime().asSeconds() * 1.f * M_PI) + alphaOffset;
	_settingsTitle.setFillColor(sf::Color(rustColor.r, rustColor.g, rustColor.b, alpha));

	for (auto& pair : _uiElements)
	{
		if (pair.first.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(Engine::getWindow()))))
		{
			sf::Event event;
			Engine::getWindow().waitEvent(event);
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (pair.second.getString() == _settingsNames[0])
				{
					if (_settings[_settingsNames[0]] == false)
					{
						_settings[pair.second.getString()] = true;
						Engine::getWindow().create(sf::VideoMode(Engine::getWindowSize()), "CARTHARSIS", sf::Style::Fullscreen);
					}
					else
					{
						_settings[pair.second.getString()] = false;
						Engine::getWindow().create(sf::VideoMode(Engine::getWindowSize()), "CARTHARSIS", sf::Style::Resize);
					}
				}

				if (pair.second.getString() == _settingsNames[1])
				{
					_settings[_settingsNames[1]] = !_settings[_settingsNames[1]];
					Engine::getWindow().setVerticalSyncEnabled(_settings[_settingsNames[1]]);
				}

				if (pair.second.getString() == _settingsNames[2])
				{
					_settings[_settingsNames[2]] = !_settings[_settingsNames[2]];
				}

				if (pair.second.getString() == _settingsNames[3])
				{
					_settings[_settingsNames[3]] = !_settings[_settingsNames[3]];
				}

			}
		}
		else
		{
			if (_settings[pair.second.getString()] == false)
				pair.first.setFillColor(sf::Color::Transparent);
			else
				pair.first.setFillColor(sf::Color::Red);
		}
	}

	
	UpdatePositions();
}

void SettingsScene::Render()
{
	Engine::getWindow().draw(_settingsTitle);
	for (auto& pair : _uiElements)
	{
		Engine::getWindow().draw(pair.first);
		Engine::getWindow().draw(pair.second);
	}
	
}


