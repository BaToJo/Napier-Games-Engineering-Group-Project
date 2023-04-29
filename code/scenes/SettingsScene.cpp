#include "SettingsScene.h"



void SettingsScene::UpdatePositions()
{
	_settingsTitle.setPosition(sf::Vector2f(Engine::getWindowSize().x / 2.f, 40.f));

	for (int i = 0; i < _listOfTexts.size(); i++)
	{
		_listOfTexts[i].setPosition(sf::Vector2f(
			(Engine::getWindowSize().x / 2.f) - _settingsTitle.getGlobalBounds().width,
			200.f + (i * (_listOfTexts[i].getGlobalBounds().height + 50.f)
		)));
	}

	for (int i = 0; i < _listOfButtons.size(); i++)
	{
		_listOfButtons[i].setPosition(sf::Vector2f(
			(Engine::getWindowSize().x / 2.f) + _settingsTitle.getGlobalBounds().width / 2.f,
			_listOfTexts[i].getPosition().y + _listOfTexts[i].getGlobalBounds().height / 2.f + _listOfButtons[i].getSize().y / 4.f)
		);
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

			buttonText.setFillColor(rustColor);
			_listOfTexts.push_back(buttonText);
		}

		_listOfTexts[0].setString("FULLSCREEN: ");
		_listOfTexts[1].setString("VSYNC: ");
		_listOfTexts[2].setString("BGM: ");
		_listOfTexts[3].setString("EFFECTS: ");

		for (int i = 0; i < _listOfTexts.size(); i++)
		{
			_listOfTexts[i].setPosition(sf::Vector2f(
				(Engine::getWindowSize().x / 2.f) - _settingsTitle.getGlobalBounds().width,
				100.f + (i * (_listOfTexts[i].getGlobalBounds().height + 50.f)
					)));
		}
	}

	for (int i = 0; i < 4; i++)
	{
		sf::RectangleShape rect;
		rect.setSize(sf::Vector2f(20.f, 20.f));
		rect.setOrigin(sf::Vector2f(rect.getSize().x / 2.f, rect.getSize().y / 2.f));
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(1.2f);
		rect.setPosition(sf::Vector2f(
			(Engine::getWindowSize().x / 2.f) + _settingsTitle.getGlobalBounds().width / 2.f,
			_listOfTexts[i].getPosition().y + _listOfTexts[i].getGlobalBounds().height / 2.f + rect.getSize().y / 4.f)
		);

		_listOfButtons.push_back(rect);
	}

}

void SettingsScene::Unload()
{
	_listOfTexts.clear();
	Scene::Unload();
}

void SettingsScene::Update(const double& dt)
{
	const float alpha = alphaAmplitude * std::sin(settingsClock.getElapsedTime().asSeconds() * 1.f * M_PI) + alphaOffset;
	_settingsTitle.setFillColor(sf::Color(rustColor.r, rustColor.g, rustColor.b, alpha));

	for (int i = 0; i < _listOfButtons.size(); i++)
	{

		if (_listOfButtons[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(Engine::getWindow()))))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				_listOfButtons[i].setFillColor(sf::Color::Red);
				if (_listOfTexts[i].getString() == "FULLSCREEN: ")
				{
					Engine::getWindow().create(sf::VideoMode(Engine::getWindowSize()), "CARTHARSIS", sf::Style::Fullscreen);
				}
			}
		}
		else
		{
			_listOfButtons[i].setFillColor(sf::Color::Transparent);
		}
	}

	UpdatePositions();
}

void SettingsScene::Render()
{
	Engine::getWindow().draw(_settingsTitle);
	for (auto& t : _listOfTexts)
		Engine::getWindow().draw(t);
	for (auto& b : _listOfButtons)
		Engine::getWindow().draw(b);
}
