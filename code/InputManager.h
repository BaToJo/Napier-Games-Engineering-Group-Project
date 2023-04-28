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
	float deadzoneX = 0.2f;
	float deadzoneY = 0.2f;

	bool isControllerConnected = false;
	
	int gamepadMovement;
public:
	InputManager();
	std::vector<sf::Keyboard::Key> keyboardControls;

	void RebindKeyboard(const int index, const sf::Keyboard::Key key);

	float IsMovingForward();
	float IsMovingBack();
	float IsMovingLeft();
	float IsMovingRight();

	float CalculateYAxisValueWithDeadZone();
	float CalculateXAxisValueWithDeadZone();

	void IsControllerConnected();
};