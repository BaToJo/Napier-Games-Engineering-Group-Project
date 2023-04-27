#pragma once

#include <iostream>
#include <Windows.h>
#include <Xinput.h>
#include <vector>
#include <SFML/Window/Keyboard.hpp>

#pragma comment(lib, "XInput.lib")

class InputManager 
{
private:
	XINPUT_STATE state;
	float deadzoneX;
	float deadzoneY;

	bool isControllerConnected = false;
	
	int gamepadMovement;
public:
	InputManager();
	std::vector<sf::Keyboard::Key> keyboardControls;

	void RebindKeyboard(const int index, const sf::Keyboard::Key key);

	bool IsMovingForward();
	bool IsMovingBack();
	bool IsMovingLeft();
	bool IsMovingRight();


	void IsControllerConnected();
};