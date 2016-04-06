#include "UIButton.h"
#include "UIPalette.h"

namespace WebEngine
{
	UIButton::UIButton()
	{
	}


	UIButton::~UIButton()
	{
	}

	glm::vec2 UIButton::getAbsolutePosition()
	{
		return m_owner->getPosition() + m_relativePosition;
	}

	void UIButton::setCurrentState(UIButtonState& buttonState)
	{
		m_currentState = buttonState;
	}

}
