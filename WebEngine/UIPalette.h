#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "GLTexture.h"
#include "UIButton.h"
#include "Camera2D.h"

namespace WebEngine 
{
	class UIPalette
	{
	public:
		UIPalette();
		~UIPalette();

		//Accessors
		const glm::vec2& getPosition() const { return m_position; }
		const glm::vec2& getDimensions() const { return m_dimensions;  }

	private:
		//Position and dimensions in screen coordinates
		glm::vec2 m_position;
		glm::vec2 m_dimensions;

		//Texture
		GLTexture m_texture;

		std::vector<UIButton> m_buttons;
	};
}


