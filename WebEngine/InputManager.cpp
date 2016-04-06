#include "InputManager.h"
#include "Err.h"
#include <iostream>

namespace WebEngine{

	InputManager::InputManager() : _mouseCoords(0, 0)
	{
	}
	/*
	TODO:: FIX MEMORY LEAK!!!!
	*/
	InputManager::~InputManager()
	{

	}

	void InputManager::update()
	{
		//Loop through key map using for each, copy to previous
		for (auto& it :_keyMap)
		{
			_previousKeyMap[it.first] = it.second;
		}
	}

	void InputManager::pressKey(unsigned int keyID)
	{
		//check if in map, if not create. Then set to true
		_keyMap[keyID] = true;
	}

	void InputManager::releaseKey(unsigned int keyID)
	{
		_keyMap[keyID] = false;
	}

	void InputManager::setMouseCoords(float x, float y)
	{
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}

	bool InputManager::isKeyDown(unsigned int keyID)
	{
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end())
		{
			return it->second;
		}
		else
		{
			return false;
		}
	}

	bool InputManager::isKeyPressed(unsigned int keyID)
	{
		//Check if pressed this frame and not pressed next frame
		if (isKeyDown(keyID) && !wasKeyDown(keyID))
		{
			return true;
		}
		return false;
	}

	bool InputManager::isKeyReleased(unsigned int keyID)
	{
		if (!isKeyDown(keyID) && wasKeyDown(keyID))
		{
			return true;
		}
	}

	bool InputManager::wasKeyDown(unsigned int keyID)
	{
		auto it = _previousKeyMap.find(keyID);
		if (it != _previousKeyMap.end())
		{
			return it->second;
		}
		else
		{
			return false;
		}
	}
}