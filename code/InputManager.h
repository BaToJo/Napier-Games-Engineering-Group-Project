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
	
	// Deadzone values
	float deadzoneX = 0.2f;
	float deadzoneY = 0.6f;

	bool isControllerConnected = false;
	bool isDPadEnabled = false;
	int gamepadMovement;


	// Calculation function
	float CalculateYAxisValueWithDeadZone();
	float CalculateXAxisValueWithDeadZone();

public:
	InputManager();
	std::vector<sf::Keyboard::Key> keyboardControls;

	void RebindKeyboard(const int index, const sf::Keyboard::Key key);

	// Movement Functions
	float IsMovingForward();
	float IsMovingBack();
	float IsMovingLeft();
	float IsMovingRight();

	// Checks
	bool IsMoving();
	void IsControllerConnected();
};