#include "DebugRenderer.h"

const float PI = 3.14159265359;

namespace{ //Prevent name collision
	//Shader code with raw string
	const char* VERT_SRC = R"(#version 130
	//The vertex shader operates on each vertex

	//input data from the VBO. Each vertex is 2 floats
	in vec2 vertexPosition;
in vec4 vertexColor;

out vec2 fragmentPosition;
out vec4 fragmentColor;

uniform mat4 P;

void main() {
	//Set the x,y position on the screen
	gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
	//the z position is zero since we are in 2D
	gl_Position.z = 0.0;

	//Indicate that the coordinates are normalized
	gl_Position.w = 1.0;

	fragmentPosition = vertexPosition;

	fragmentColor = vertexColor;
})";

	const char* FRAG_SRC = R"(
#version 130
//The fragment shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
in vec4 fragmentColor;

//This is the 3 component float vector that gets outputted to the screen
//for each pixel.
out vec4 color;

void main() {

    color = fragmentColor;
})";

}

namespace WebEngine {
	DebugRenderer::DebugRenderer()
	{ //Empty
	}


	DebugRenderer::~DebugRenderer()
	{
		close();
	}

	void DebugRenderer::init()
	{
		//Shader init
		mProgram.compileShadersFromSource(VERT_SRC, FRAG_SRC);
		mProgram.addAttribute("vertexPosition");
		mProgram.addAttribute("vertexColor");
		mProgram.linkShaders();

		//set up buffers
		glGenVertexArrays(1, &mVAO);
		glGenBuffers(1, &mVBO);
		glGenBuffers(1, &mIBO);

		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

		//Add pointer to position attrib to vertex attrib array
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, position));
		//Add pointer to color attrib
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));


		//Unbind vertex array and buffers
		glBindVertexArray(0);
	}

	void DebugRenderer::end()
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		//Orphan buffer for speed
		glBufferData(GL_ARRAY_BUFFER, mVertexVector.size() * sizeof(DebugVertex), nullptr, GL_DYNAMIC_DRAW);
		//Upload data
		glBufferSubData(GL_ARRAY_BUFFER, 0, mVertexVector.size() * sizeof(DebugVertex), mVertexVector.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
		//Orphan buffer for speed
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexVector.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
		//Upload data
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, mIndexVector.size() * sizeof(GLuint), mIndexVector.data());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		mNumElements = mIndexVector.size();
		mIndexVector.clear();
		mVertexVector.clear();
	}

	void DebugRenderer::drawLine(const glm::vec2& midPoint, float length, ColorRGBA8& color, float angle)
	{
		int i = mVertexVector.size(); // first vertex we are modifying
		mVertexVector.resize(mVertexVector.size() + 2);

		glm::vec2 left = glm::vec2( length / 2.0f*cos(angle + PI), (length / 2.0f)*sin(angle + PI));
		 
		glm::vec2 right = glm::vec2( length / 2.0f*cos(angle), (length / 2.0f)*sin(angle));

		mVertexVector[i].position = left + midPoint;
		mVertexVector[i + 1].position = right + midPoint;

		for (int j = i; j < i + 2; j++)
		{
			mVertexVector[j].color = color;
		}

		//It woulld be faster to call resize
		mIndexVector.reserve(mIndexVector.size() + 2);

		//Grab indeces for each line of box
		mIndexVector.push_back(i);
		mIndexVector.push_back(i + 1);
	}
	//Overload drawLine -> takes in endpoins and line color
	void DebugRenderer::drawLine(const glm::vec2& lEndpoint, const glm::vec2& rEndpoint, ColorRGBA8& color)
	{
		int i = mVertexVector.size(); // first vertex we are modifying
		mVertexVector.resize(mVertexVector.size() + 2);

		mVertexVector[i].position = lEndpoint;
		mVertexVector[i + 1].position = rEndpoint;

		for (int j = i; j < i + 2; j++)
		{
			mVertexVector[j].color = color;
		}

		//It woulld be faster to call resize
		mIndexVector.reserve(mIndexVector.size() + 2);

		//Grab indeces for each line of box
		mIndexVector.push_back(i);
		mIndexVector.push_back(i + 1);
	}

	void DebugRenderer::drawBox(const glm::vec4& destRect, const ColorRGBA8& color, float angle)
	{
		int i = mVertexVector.size(); // first vertex we are modifying
		mVertexVector.resize(mVertexVector.size() + 4 );

		glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

		//Find rotated points
		glm::vec2 tr = glm::vec2(halfDims.x * cos(angle) - halfDims.y * sin(angle),
								 halfDims.x * sin(angle) + halfDims.y * cos(angle));

		glm::vec2 tl = glm::vec2(-halfDims.x * cos(angle) - halfDims.y * sin(angle),
								 -halfDims.x * sin(angle) + halfDims.y * cos(angle));

		glm::vec2 positionOffset(destRect.x, destRect.y);

		//Add vertices to vector
		mVertexVector[i].position = tl + positionOffset + halfDims;
		mVertexVector[i + 1].position = -tr + positionOffset + halfDims;
		mVertexVector[i + 2].position = -tl + positionOffset + halfDims;
		mVertexVector[i + 3].position = tr + positionOffset + halfDims;
		
		for ( int j = i; j < i + 4; j++)
		{
			mVertexVector[j].color = color;
		}
		//It woulld be faster to call resize
		mIndexVector.reserve(mIndexVector.size() + 8);

		//Grab indeces for each line of box
		mIndexVector.push_back(i);
		mIndexVector.push_back(i + 1);

		mIndexVector.push_back(i + 1);
		mIndexVector.push_back(i + 2);

		mIndexVector.push_back(i + 2);
		mIndexVector.push_back(i + 3);

		mIndexVector.push_back(i + 3);
		mIndexVector.push_back(i);
	}

	void DebugRenderer::drawCircle(const glm::vec2 center, const ColorRGBA8& color, float radius)
	{
		//TODO make proportional to size
		static const int NUM_VERTS = 100;

		int start = mVertexVector.size();
		mVertexVector.resize(mVertexVector.size() + NUM_VERTS);
		for (int i = 0; i < NUM_VERTS; i++)
		{
			float angle = ((float)i / (float)NUM_VERTS) * PI * 2.0f;
			mVertexVector[start + i].position.x = cos(angle) * radius + center.x;
			mVertexVector[start + i].position.y = sin(angle) * radius + center.y;
			mVertexVector[start + i].color = color;
		}
		
		//Set up index vector to hold vertex indices
		mIndexVector.reserve(mVertexVector.size() + NUM_VERTS * 2);

		//Loop through all but last of vertices
		for (int i = 0; i < NUM_VERTS - 1; i++)
		{
			mIndexVector.push_back(start + i);
			mIndexVector.push_back(start + i + 1);
		}
		//last index push back
		mIndexVector.push_back(start + NUM_VERTS - 1);
		mIndexVector.push_back(start);
	}

	void DebugRenderer::render(const glm::mat4& projectionMatrix, float lineWidth)
	{
		mProgram.use();

		GLuint projectionUniform = mProgram.getUniformLocation("P");
		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

		glLineWidth(lineWidth);
		glBindVertexArray(mVAO);

		glDrawElements(GL_LINES, mNumElements, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		mProgram.unuse();
	}

	void DebugRenderer::close()
	{
		if (mVAO)
		{
			glDeleteVertexArrays(1, &mVAO);
		}
		if (mVBO)
		{
			glDeleteBuffers(1, &mVBO);
		}
		if (mVAO)
		{
			glDeleteBuffers(1, &mIBO);
		}
		mProgram.close();
	}

}