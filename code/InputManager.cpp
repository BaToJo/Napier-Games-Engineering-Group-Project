#include "InputManager.h"

// The code in this manager is partly taken from https://katyscode.wordpress.com/2013/08/30/xinput-tutorial-part-1-adding-gamepad-support-to-your-windows-game/#:~:text=To%20use%20this%2C%20you%20need,lib%20as%20the%20linker%20input).
XINPUT_STATE InputManager::state;
float InputManager::deadzoneX = 0.2f;
float InputManager::deadzoneY = 0.2f;
bool InputManager::isControllerConnected = false;
bool InputManager::isDPadEnabled = true;

// Setting up a list of default controls
std::vector<sf::Keyboard::Key> InputManager::keyboardControls{
	sf::Keyboard::W,
	sf::Keyboard::A,
	sf::Keyboard::S,
	sf::Keyboard::D
};

// Function to rebind the current controls
void InputManager::RebindKeyboard(const int index, const sf::Keyboard::Key key)
{
	// If the index is out of bounds, just return
	if (index >= keyboardControls.size())
		return;

	// If the key is unkown, just return
	if (key == sf::Keyboard::Unknown)
		return;

	// Replacing the control at given index with given key
	keyboardControls[index] = key;
}

float InputManager::IsMovingForward()
{
	IsControllerConnected();
	// State machine that handles how the input movement is processed based on what is currently connected or used in terms of dpad or stick.
	if (!isControllerConnected)
	{
		if (sf::Keyboard::isKeyPressed(keyboardControls[0]))
			return 1;
		else
			return 0;
	}
	else if (isControllerConnected && !isDPadEnabled)
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
	IsControllerConnected();
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
	IsControllerConnected();
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
	IsControllerConnected();
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

// This function is used to check if the player is currently moving. It is going to be used in the player physics code
bool InputManager::IsMoving()
{
	IsControllerConnected();
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

void InputManager::IsControllerConnected()
{
	// Checks if a controller is connected
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
	// Code taken from reference. This snippet of code will calculate how much a stick is moved in a range between [-1, 1].
	// This will also include a deadzone calculation that will avoid the stick from being far too sensitive
	float normLY = fmaxf(-1, (float)state.Gamepad.sThumbLY / 32767);

	float leftStickY = (abs(normLY) < deadzoneY ? 0 : normLY);
	leftStickY = (abs(normLY) < deadzoneY ? 0 : (abs(normLY) - deadzoneY) * (normLY / abs(normLY)));

	if (deadzoneY > 0)
		leftStickY /= 1 - deadzoneY;

	return leftStickY;
}

float InputManager::CalculateXAxisValueWithDeadZone()
{
	// Same as before but on different axis
	float normLX = fmaxf(-1, (float)state.Gamepad.sThumbLX / 32767);
	float leftStickX = (abs(normLX) < deadzoneX ? 0 : normLX);

	leftStickX = (abs(normLX) < deadzoneX ? 0 : (abs(normLX) - deadzoneX) * (normLX / abs(normLX)));

	if (deadzoneX > 0)
		leftStickX /= 1 - deadzoneX;

	return leftStickX;
}
