#pragma once
#include "UIButtonState.h"

namespace WebEngine 
{
	class UIButton;

	class UIInactiveButtonState : public UIButtonState
	{
	public:
		UIInactiveButtonState();
		~UIInactiveButtonState();

		virtual void handleInput(UIButton& ownerButton, InputManager& inputManager);

		virtual void update();
	};

}

