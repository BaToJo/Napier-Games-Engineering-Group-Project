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
	static float deadzoneX;
	static float deadzoneY;

	static bool isControllerConnected;
	static bool isDPadEnabled;
	static int gamepadMovement;

public:
	InputManager() = delete;
	static std::vector<sf::Keyboard::Key> keyboardControls;

	static void RebindKeyboard(const int index, const sf::Keyboard::Key key);

	static float IsMovingForward();
	static float IsMovingBack();
	static float IsMovingLeft();
	static float IsMovingRight();

	static float CalculateYAxisValueWithDeadZone();
	static float CalculateXAxisValueWithDeadZone();

	static bool IsMoving();
	static void IsControllerConnected();
};