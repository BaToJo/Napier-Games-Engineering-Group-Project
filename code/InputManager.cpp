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
	else if(isControllerConnected && !isDPadEnabled)
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
	else if (isControllerConnected && isDPadEnabled)
	{
		if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0)
		{
			return 1;
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
	else if (isControllerConnected && !isDPadEnabled)
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
	else if (isControllerConnected && isDPadEnabled)
	{
		if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0)
		{
			return 1;
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
	else if (isControllerConnected && !isDPadEnabled)
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
	else if (isControllerConnected && isDPadEnabled)
	{
		if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0)
		{
			return 1;
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
	else if (isControllerConnected && !isDPadEnabled)
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
	else if (isControllerConnected && isDPadEnabled)
	{
		if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

bool InputManager::IsMoving()
{
	if (!isControllerConnected)
	{
		for (int i = 0; i < keyboardControls[i]; i++)
		{
			if (sf::Keyboard::isKeyPressed(keyboardControls[i]))
			{
				return true;
			}
			
		}

		return false;
	}
	else if (isControllerConnected && !isDPadEnabled)
	{
		float leftStickX = CalculateXAxisValueWithDeadZone();
		float leftstickY = CalculateYAxisValueWithDeadZone();

		if (leftStickX == 0 && leftstickY == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}

//bool InputManager::isDrifting()
//{
//	if (!isControllerConnected)
//	{
//		if (sf::Keyboard::isKeyPressed(keyboardControls[1]) || sf::Keyboard::isKeyPressed(keyboardControls[3]))
//		{
//			return true;
//		}
//		else 
//		{
//			return false;
//		}
//	}
//	else if (isControllerConnected && !isDPadEnabled)
//	{
//		float leftStickX = CalculateXAxisValueWithDeadZone();
//		if (leftStickX == 0)
//		{
//			return false;
//		}
//		else
//		{
//			return true;
//		}
//	}
//	return false;
//}

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
