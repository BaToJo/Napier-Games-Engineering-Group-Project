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
	static XINPUT_STATE state;
	// Deadzone values
	static float deadzoneX;
	static float deadzoneY;

	static bool isControllerConnected;
	static bool isDPadEnabled;

public:
	InputManager() = delete;
	static std::vector<sf::Keyboard::Key> keyboardControls;

	static void RebindKeyboard(const int index, const sf::Keyboard::Key key);

	// Movement functions
	static float IsMovingForward();
	static float IsMovingBack();
	static float IsMovingLeft();
	static float IsMovingRight();

	// Calculate functions
	static float CalculateYAxisValueWithDeadZone();
	static float CalculateXAxisValueWithDeadZone();

	// Checks
	static bool IsMoving();
	static void IsControllerConnected();
};