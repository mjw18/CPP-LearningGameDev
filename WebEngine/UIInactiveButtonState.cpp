#include "UIInactiveButtonState.h"
#include "UIButton.h"

namespace WebEngine 
{
	UIInactiveButtonState::UIInactiveButtonState()
	{
	}


	UIInactiveButtonState::~UIInactiveButtonState()
	{
	}

	void UIInactiveButtonState::handleInput(UIButton& ownerButton, InputManager& inputManager)
	{

		glm::vec2 mouseCoords;// = m_camera->screenToWorld(inputManager.getMouseCoords());

		if (mouseCoords.x >= (ownerButton.getAbsolutePosition().x) && mouseCoords.x <= (ownerButton.getAbsolutePosition().x + ownerButton.getDimensions().x))
		{
			if (mouseCoords.y >= ownerButton.getAbsolutePosition().y && mouseCoords.y <= (ownerButton.getAbsolutePosition().y + ownerButton.getDimensions().y))
			{
				ownerButton.setCurrentState(ownerButton.getHoverState());
			}
		}

	}

	void update()
	{

	}
}