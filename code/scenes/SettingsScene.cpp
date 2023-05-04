#include "SettingsScene.h"

#include "../game.h"
#include "../../lib_JSON_reader/TileMap_Importer.h"
#include "../InputManager.h"

// This function updates the position of the buttons relative to the current screen so that everything will be in place regardless of the size of the window
void SettingsScene::UpdatePositions()
{
	_settingsTitle.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 80.f));

	for (int i = 0; i < _uiElements.size(); i++)
	{
		if (i < 7)
		{
			_uiElements[i].first.setPosition(sf::Vector2f(
				(Engine::getWindowSize().x / 2.f) + _settingsTitle.getGlobalBounds().width / 2.f,
				_uiElements[i].second.getPosition().y + _uiElements[i].second.getGlobalBounds().height / 2.f + _uiElements[i].first.getSize().y / 4.f));
		}
		else
		{
			_uiElements[i].first.setPosition(sf::Vector2f(
				(Engine::getWindowSize().x / 2.f),
				_uiElements[i].second.getPosition().y + _uiElements[i].second.getGlobalBounds().height / 2.f + _uiElements[i].first.getSize().y / 4.f));
		}

		_uiElements[i].second.setPosition(sf::Vector2f(
			(Engine::getWindowSize().x / 2.f) - _settingsTitle.getGlobalBounds().width / 2.f,
			180.f + (i * (_uiElements[i].second.getGlobalBounds().height + 20.f))));
	}
}

// This function will update the currently displayed keys
void SettingsScene::UpdateKeys()
{
	for (int i = 0; i < InputManager::keyboardControls.size(); i++)
	{
		int uiIndex = _uiElements.size() - (4 - i);
		auto scan = sf::Keyboard::delocalize(InputManager::keyboardControls[i]);

		_uiElements[uiIndex].second.setString(sf::Keyboard::getDescription(scan).toAnsiString());
	}
}

// This function will check the current window resolution and highlight the corresponding checkbox
void SettingsScene::UpdateResolution()
{

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
}

void SettingsScene::Load()
{
	TileMap_Importer::Settings_ReadFromFile("res/player_settings/player_settings.json");

	Engine::getWindow().setView(Engine::getWindow().getDefaultView());

	// The settings clock is used to animate the title
	settingsClock = sf::Clock();

	// Creating the title
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

	// This will create every setting text with every button corresponding to it
	if (_buttonFont.loadFromFile("res/fonts/Kanit-Light.ttf"))
	{
		// Setting the properties of every text in the scene.
		sf::Text buttonText;
		buttonText.setFont(_buttonFont);
		buttonText.setCharacterSize(30);
		buttonText.setOutlineColor(sf::Color(255, 255, 255, 0));
		buttonText.setOutlineThickness(1.2f);
		buttonText.setFillColor(rustColor);

		// Setting the properties of every rectangle in the scene
		sf::RectangleShape rect;
		rect.setSize(sf::Vector2f(20.f, 20.f));
		rect.setOrigin(sf::Vector2f(rect.getSize().x / 2.f, rect.getSize().y / 2.f));
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(1.2f);

		// Setting the text of every button. The position and the displayed text is the only thing that really differs.
		for (int i = 0; i < 7; i++)
		{
			buttonText.setString(_settings[i].first);
			_uiElements.push_back(std::pair<sf::RectangleShape, sf::Text>(rect, buttonText));
		}

		// Every button in the input manager is delocalized to get the corresponding string value and then it's added a text element in the UI
		for (int i = 0; i < InputManager::keyboardControls.size(); i++)
		{
			auto scan = sf::Keyboard::delocalize(InputManager::keyboardControls[i]);
			buttonText.setString(sf::Keyboard::getDescription(scan).toAnsiString());
			_uiElements.push_back(std::pair<sf::RectangleShape, sf::Text>(rect, buttonText));
		}
	}
}

void SettingsScene::Unload()
{

	// Cleaning up the elements
	_uiElements.clear();
	// Write settings to disk.
	TileMap_Importer::Settings_WriteToFile("res/player_settings/player_settings.json", vsync_enabled, fullscreen_enabled, backgroundMusic_enabled, soundEffects_enabled, TileMap_Importer::currentResolution);
	Scene::Unload();
}

void SettingsScene::Update(const double& dt)
{
	// This snippet of code makes the title pulse
	const float alpha = alphaAmplitude * std::sin(settingsClock.getElapsedTime().asSeconds() * 1.f * M_PI) + alphaOffset;
	_settingsTitle.setFillColor(sf::Color(rustColor.r, rustColor.g, rustColor.b, alpha));

	// If the code is in a rebinding state it will wait for a proper keyboard input and it will change the corresponding keybind
	if (isRebinding && currentKeyPressed != sf::Keyboard::Unknown)
	{
		InputManager::RebindKeyboard(indexRebind, currentKeyPressed);
		currentKeyPressed = sf::Keyboard::Unknown;
		indexRebind = 0;
		isRebinding = false;
	}

	int i = 0;
	// Checking every pair in the elements. A pair is composed by a "checkbox" (Rectangle Shape) and the corresponding sf::Text
	for (auto& pair : _uiElements)
	{

		// First of all we check if we're actually inside one of the checkboxes
		if (pair.first.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(Engine::getWindow()))))
		{
			// If the event in the event handler has fired, this flag will be set to true
			if (isMousePressed)
			{
				// This is a big state machine that will check what have we currently clicked and how is that going to affect the settings
				// The _settings list is simply a list of pairs that will have a string and a boolean value that will handle different settings across the entire game
				if (pair.second.getString() == _settings[0].first)
				{
					if (_settings[0].second == false)
					{
						fullscreen_enabled = true;
						_settings[0].second = true;
						Engine::getWindow().create(sf::VideoMode(Engine::getWindowSize()), "CARTHARSIS", sf::Style::Fullscreen);
					}
					else
					{
						fullscreen_enabled = false;
						_settings[0].second = false;
						Engine::getWindow().create(sf::VideoMode(Engine::getWindowSize()), "CARTHARSIS", sf::Style::Resize);
					}
				}

				// VSync
				if (pair.second.getString() == _settings[1].first)
				{
					_settings[1].second = !_settings[1].second;
					Engine::getWindow().setVerticalSyncEnabled(_settings[1].second);
					vsync_enabled = _settings[1].second;
				}

				// Background Music
				if (pair.second.getString() == _settings[2].first)
				{
					_settings[2].second = !_settings[2].second;
					backgroundMusic_enabled = _settings[2].second;
				}

				// Sound Effects
				if (pair.second.getString() == _settings[3].first)
				{
					_settings[3].second = !_settings[3].second;
					soundEffects_enabled = _settings[3].second;
				}

				// Screen resolution
				if (pair.second.getString() == _settings[4].first || pair.second.getString() == _settings[5].first || pair.second.getString() == _settings[6].first)
				{
					// This snippet of code simply takes in the string "width x height" and subdivides it into "width" "x" "height". We only care about width and height, the x is discarded.
					std::stringstream ss(pair.second.getString());
					if (ss.str() != "")
					{
						//TileMap_Importer::currentResolution = ss.str();
						unsigned int width, height;
						char x;
						ss >> width >> x >> height;
						sf::Vector2u newSize = sf::Vector2u(width, height);
						Engine::getWindow().create(sf::VideoMode(newSize), "CARTHARSIS");
						ss.clear();
					}


				}

				// This checks what key are we changing in the keyboard controls based on the corresponding sf::Text
				for (int j = 0; j < InputManager::keyboardControls.size(); j++)
				{
					auto scan = sf::Keyboard::delocalize(InputManager::keyboardControls[j]);
					if (pair.second.getString() == sf::Keyboard::getDescription(scan).toAnsiString())
					{
						// We store the state and the index of the keyboardControls that we are going to change
						isRebinding = true;
						indexRebind = j;
					}
				}
				// We only want one mouse click to happen every frame
				isMousePressed = false;
			}
		}
		else
		{
			// This fills the various settings based on their state. If they are false they are transparent, if true they are red
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
			std::stringstream ss(_settings[i].first);
			if (ss.str() != "")
				TileMap_Importer::currentResolution = ss.str();
		}
		else
		{
			_settings[i].second = false;
		}
	}
	UpdateResolution();
	UpdateKeys();
	UpdatePositions();

}
	void SettingsScene::Render()
	{
		Engine::getWindow().draw(_settingsTitle);
		for (auto &pair : _uiElements)
		{
			Engine::getWindow().draw(pair.first);
			Engine::getWindow().draw(pair.second);
		}
		Engine::getWindow().draw(_controlsText);
	}

	void SettingsScene::HandleEvents()
	{
		sf::Event event;
		while (Engine::getWindow().pollEvent(event))
		{
			if (event.type == sf::Event::MouseButtonPressed)
			{
				isMousePressed = true;
			}
			if (event.type == sf::Event::KeyPressed && isRebinding)
			{
				currentKeyPressed = event.key.code;
			}
			if (event.type == sf::Event::Closed)
			{
				Engine::getWindow().close();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				Engine::ChangeScene(&menuScene);
			}
			if (event.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(sf::Vector2f(0.f, 0.f), sf::Vector2f(event.size.width, event.size.height));
				Engine::getWindow().setView(sf::View(visibleArea));
			}
		}
	}