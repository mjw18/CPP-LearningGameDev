#pragma once
#include "UIButtonState.h"

namespace WebEngine
{
	class UIHoverButtonState :
		public UIButtonState
	{
	public:
		UIHoverButtonState();
		~UIHoverButtonState();

		virtual void handleInput(InputManager& inputManager);
	};
}


