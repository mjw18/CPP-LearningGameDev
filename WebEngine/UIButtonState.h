#pragma once

/*
* UIButtonState interface
*/
#include "InputManager.h"

class UIButton;

namespace WebEngine 
{
	class UIButtonState
	{
	public:
		UIButtonState();
		~UIButtonState();

		virtual void handleInput(InputManager& inputManager);
		virtual void uodate();
	};

}

