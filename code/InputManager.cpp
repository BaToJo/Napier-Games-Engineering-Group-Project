#include "InputManager.h"

InputManager::InputManager()
{
	// Setting up a list of default controls
	keyboardControls = {
		sf::Keyboard::W,
		sf::Keyboard::A,
		sf::Keyboard::S,
		sf::Keyboard::D
	};
	
	IsControllerConnected();
}


void InputManager::RebindKeyboard(const int index, const sf::Keyboard::Key key)
{
	if (index >= keyboardControls.size()) return;

	if (key == sf::Keyboard::Unknown) return;

	keyboardControls[index] = key;
}

float InputManager::IsMovingForward()
{
	if (!isControllerConnected)
	{
		if (sf::Keyboard::isKeyPressed(keyboardControls[0]))
			return 1;
		else
			return 0;
	}
	else
	{
		float leftStickY = CalculateYAxisValueWithDeadZone();

		if (leftStickY > 0)
		{
			return leftStickY;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

float InputManager::IsMovingBack()
{
	if (!isControllerConnected)
	{
		if (sf::Keyboard::isKeyPressed(keyboardControls[2]))
			return 1;
		else
			return 0;
	}
	else
	{
		float leftStickY = CalculateYAxisValueWithDeadZone();

		if (leftStickY < 0)
		{
			return leftStickY;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

float InputManager::IsMovingLeft()
{
	if (!isControllerConnected)
	{
		if (sf::Keyboard::isKeyPressed(keyboardControls[1]))
			return 1;
		else
			return 0;
	}
	else
	{
		float leftStickX = CalculateXAxisValueWithDeadZone();

		if (leftStickX < 0)
		{
			return leftStickX;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

float InputManager::IsMovingRight()
{
	if (!isControllerConnected)
	{
		if (sf::Keyboard::isKeyPressed(keyboardControls[3]))
			return 1;
		else
			return 0;
	}
	else
	{
		float leftStickX = CalculateXAxisValueWithDeadZone();

		if (leftStickX > 0)
		{
			return leftStickX;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

void InputManager::IsControllerConnected()
{
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	if (XInputGetState(0, &state) == ERROR_SUCCESS)
	{
		isControllerConnected = true;
	}
	else 
	{
		isControllerConnected = false;
	}
}


float InputManager::CalculateYAxisValueWithDeadZone()
{
	float normLY = fmaxf(-1, (float)state.Gamepad.sThumbLY / 32767);

	float leftStickY = (abs(normLY) < deadzoneY ? 0 : normLY);
	leftStickY = (abs(normLY) < deadzoneY ? 0 : (abs(normLY) - deadzoneY) * (normLY / abs(normLY)));

	if (deadzoneY > 0) leftStickY /= 1 - deadzoneY;

	return leftStickY;
}

float InputManager::CalculateXAxisValueWithDeadZone()
{
	float normLX = fmaxf(-1, (float)state.Gamepad.sThumbLX / 32767);
	float leftStickX = (abs(normLX) < deadzoneX ? 0 : normLX);

	leftStickX = (abs(normLX) < deadzoneX ? 0 : (abs(normLX) - deadzoneX) * (normLX / abs(normLX)));

	if (deadzoneX > 0) leftStickX /= 1 - deadzoneX;

	return leftStickX;
}
