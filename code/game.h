#pragma once

#include "scenes/GameScene.h"
#include <SFML/Window/Keyboard.hpp>

extern GameScene gameScene;
extern const std::vector<sf::Keyboard::Key> controls
{
	sf::Keyboard::Up,
	sf::Keyboard::Down,
	sf::Keyboard::Left,
	sf::Keyboard::Right
};