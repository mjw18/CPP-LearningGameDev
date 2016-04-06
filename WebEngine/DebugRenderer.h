#pragma once
/*
This renderer allows for the drawing of physics objects that may not have a set sprite.
This makes testing collisions much easier.
*/
#include <vector>
#include <glm/glm.hpp>

#include "GLSLProgram.h"
#include "vertex.h"

namespace WebEngine {

	class DebugRenderer
	{
	public:
		DebugRenderer();
		~DebugRenderer();

		void init();

		void end();

		void drawLine(const glm::vec2& midPoint, float length, ColorRGBA8& color, float angle); //Horizontal at angle = 0
		void drawLine(const glm::vec2& lEndpoint, const glm::vec2& rEndpoint, ColorRGBA8& color); //Given endpoints

		void drawBox(const glm::vec4& destRect, const ColorRGBA8& color, float angle);

		void drawCircle(const glm::vec2 center, const ColorRGBA8& color, float radius);

		void render(const glm::mat4& projectionMatrix, float lineWidth);

		void close();

		//Vertex struct without uv texture coordinates
		struct DebugVertex{
			glm::vec2 position;
			ColorRGBA8 color;
		};

	private:
		GLSLProgram mProgram;
		std::vector<DebugVertex> mVertexVector;
		std::vector<GLuint> mIndexVector;
		GLuint mVBO = 0, mVAO = 0, mIBO = 0;
		int mNumElements = 0;
	};

}