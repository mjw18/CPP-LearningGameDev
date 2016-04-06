#pragma once

#include <glm/glm.hpp>
#include <WebEngine/ColorRGBA8.h>
#include <WebEngine/SpriteBatch.h>

class Light{
public:

	void draw(WebEngine::SpriteBatch& spriteBatch)
	{
		glm::vec4 destRect;
		destRect.x = position.x - size / 2.0f;
		destRect.y = position.y - size / 2.0f;
		destRect.z = size;
		destRect.w = size;

		//UV rect sets center to 0,0 lets us calc distance
		spriteBatch.draw(destRect, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), 0, 0.0f, color, 0.0f);
	}

	WebEngine::ColorRGBA8 color;
	glm::vec2 position;
	float intensity;
	float size;
};