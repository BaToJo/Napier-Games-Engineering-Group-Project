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

bool InputManager::IsMovingForward()
{
	if (!isControllerConnected)
	{
		if (sf::Keyboard::isKeyPressed(keyboardControls[0]))
			return true;
		else
			return false;
	}

	return false;
}

bool InputManager::IsMovingBack()
{
	if (!isControllerConnected)
	{
		if (sf::Keyboard::isKeyPressed(keyboardControls[2]))
			return true;
		else
			return false;
	}

	return false;
}

bool InputManager::IsMovingLeft()
{
	if (!isControllerConnected)
	{
		if (sf::Keyboard::isKeyPressed(keyboardControls[1]))
			return true;
		else
			return false;
	}

	return false;
}

bool InputManager::IsMovingRight()
{
	if (!isControllerConnected)
	{
		if (sf::Keyboard::isKeyPressed(keyboardControls[3]))
			return true;
		else
			return false;
	}

	return false;
}

void InputManager::IsControllerConnected()
{
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	if (XInputGetState(0, &state) == ERROR_SUCCESS)
	{
		isControllerConnected = true;
	}
}


