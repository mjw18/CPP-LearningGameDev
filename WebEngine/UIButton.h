#pragma once
#include <glm/glm.hpp>
#include "UIButtonState.h"
#include "UIInactiveButtonState.h"
#include "UIHoverButtonState.h"
#include "Camera2D.h"

namespace WebEngine {

	class UIPalette;

	class UIButton
	{
	public:
		UIButton();
		~UIButton();

		glm::vec2 getAbsolutePosition();

		void setCurrentState(UIButtonState& buttonState);

		//Accessor
		const glm::vec2& getRelativePos() const { return m_relativePosition; }
		const glm::vec2& getDimensions() const { return m_dimensions;  }
		UIButtonState& getHoverState() { return m_hover; }
		UIButtonState& getInactiveState() { return m_inactive; }
		const Camera2D& getCamera() { return m_camera; }

	private:
		UIPalette* m_owner;///>pointer to owner
		UIInactiveButtonState m_inactive;
		UIHoverButtonState m_hover;
		Camera2D m_camera;

		///> position and dimensions relative to owner UIPalette
		glm::vec2 m_relativePosition;
		glm::vec2 m_dimensions;

		UIButtonState m_currentState;
	};

}

