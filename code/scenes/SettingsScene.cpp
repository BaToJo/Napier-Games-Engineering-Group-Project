#include "SettingsScene.h"

#include "../game.h"
#include "../InputManager.h"

void SettingsScene::UpdatePositions()
{
	_settingsTitle.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 80.f));

	for (int i = 0; i < _uiElements.size(); i++)
	{
		if (i < 7)
		{
			_uiElements[i].first.setPosition(sf::Vector2f(
				(Engine::getWindowSize().x / 2.f) + _settingsTitle.getGlobalBounds().width / 2.f,
				_uiElements[i].second.getPosition().y + _uiElements[i].second.getGlobalBounds().height / 2.f + _uiElements[i].first.getSize().y / 4.f)
			);
		}
		else
		{
			_uiElements[i].first.setPosition(sf::Vector2f(
				(Engine::getWindowSize().x / 2.f),
				_uiElements[i].second.getPosition().y + _uiElements[i].second.getGlobalBounds().height / 2.f + _uiElements[i].first.getSize().y / 4.f)
			);
		}

		_uiElements[i].second.setPosition(sf::Vector2f(
			(Engine::getWindowSize().x / 2.f) - _settingsTitle.getGlobalBounds().width / 2.f,
			180.f + (i * (_uiElements[i].second.getGlobalBounds().height + 20.f)
		)));
	}

	//_controlsText.setPosition(sf::Vector2f(
	//	_uiElements[_uiElements.size() - 1].second.getPosition().x,
	//	_uiElements[_uiElements.size() - 1].second.getPosition().y + 40.f		
	//));
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

		_settingsTitle.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 80.f));
	}


	if (_buttonFont.loadFromFile("res/fonts/Kanit-Light.ttf"))
	{
		sf::Text buttonText;
		buttonText.setFont(_buttonFont);
		buttonText.setCharacterSize(30);
		buttonText.setOutlineColor(sf::Color(255, 255, 255, 0));
		buttonText.setOutlineThickness(1.2f);
		buttonText.setFillColor(rustColor);

		sf::RectangleShape rect;
		rect.setSize(sf::Vector2f(20.f, 20.f));
		rect.setOrigin(sf::Vector2f(rect.getSize().x / 2.f, rect.getSize().y / 2.f));
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(1.2f);

		for (int i = 0; i < 7; i++)
		{	
			buttonText.setString(_settings[i].first);
			_uiElements.push_back(std::pair<sf::RectangleShape, sf::Text>(rect, buttonText));

		}

		for (int i = 0; i < InputManager::keyboardControls.size(); i++)
		{
			auto scan = sf::Keyboard::delocalize(InputManager::keyboardControls[i]);
			buttonText.setString(sf::Keyboard::getDescription(scan).toAnsiString());
			_uiElements.push_back(std::pair<sf::RectangleShape, sf::Text>(rect, buttonText));
		}
		//_controlsText = buttonText;
		//_controlsText.setString("CONTROLS");
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

	int i = 0;
	for (auto& pair : _uiElements)
	{
		
		if (pair.first.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(Engine::getWindow()))))
		{
			sf::Event event;
			Engine::getWindow().waitEvent(event);
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (pair.second.getString() == _settings[0].first)
				{
					if (_settings[0].second == false)
					{
						_settings[0].second = true;
						Engine::getWindow().create(sf::VideoMode(Engine::getWindowSize()), "CARTHARSIS", sf::Style::Fullscreen);
					}
					else
					{
						_settings[0].second = false;
						Engine::getWindow().create(sf::VideoMode(Engine::getWindowSize()), "CARTHARSIS", sf::Style::Resize);
					}
				}

				if (pair.second.getString() == _settings[1].first)
				{
					_settings[1].second = !_settings[1].second;
					Engine::getWindow().setVerticalSyncEnabled(_settings[1].second);
				}

				if (pair.second.getString() == _settings[2].first)
				{
					_settings[2].second = !_settings[2].second;
				}

				if (pair.second.getString() == _settings[3].first)
				{
					_settings[3].second = !_settings[3].second;
				}

				if (pair.second.getString() == _settings[4].first || pair.second.getString() == _settings[5].first || pair.second.getString() == _settings[6].first)
				{
					std::stringstream ss(pair.second.getString());
					unsigned int width, height;
					char x;
					ss >> width >> x >> height;

					sf::Vector2u newSize = sf::Vector2u(width, height);
					Engine::getWindow().create(sf::VideoMode(newSize), "CARTHARSIS");

					ss.clear();
				}

			}
		}
		else
		{
			if (i < 7)
			{
				if (_settings[i].second == false)
					pair.first.setFillColor(sf::Color::Transparent);
				else
					pair.first.setFillColor(sf::Color::Red);
			}
		}
		i++;
	}

	std::string currentWindowWidth = std::to_string(Engine::getWindowSize().x);
	for (int i = _settings.size() - 3; i < _settings.size(); i++)
	{
		if (_settings[i].first.find(currentWindowWidth) != std::string::npos)
		{
			_settings[i].second = true;
		}
		else
		{
			_settings[i].second = false;
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

	Engine::getWindow().draw(_controlsText);
	
}


