/*
CHANGE SO THAT THIS DOESNT ALLOW BUTTON MASHING!!!!

*/
#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

namespace WebEngine{


	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		//Changes map from current to previous
		void update();

		//setters
		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);

		//Get Screen coordinates of mouse position
		void setMouseCoords(float x, float y);

		//Return true if key is held down
		bool isKeyDown(unsigned int keyID);
		
		//returns true if key is pressed once
		bool isKeyPressed(unsigned int keyID);

		//return true if key was down and now is not
		bool isKeyReleased(unsigned int keyID);

		//getters (const because doesnt change class variables
		glm::vec2 getMouseCoords() const { return _mouseCoords; }

	private:
		//Check if key was down using previousKeyMap
		bool wasKeyDown(unsigned int keyID);

		glm::vec2 _mouseCoords;
		std::unordered_map<unsigned int, bool> _keyMap;
		std::unordered_map<unsigned int, bool> _previousKeyMap;
	};

}