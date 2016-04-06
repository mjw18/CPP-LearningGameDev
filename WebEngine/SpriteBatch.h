#pragma once

#include <GL/glew.h>
#include <vector>

#include "vertex.h"
#include "glm/glm.hpp"

namespace WebEngine{

enum class GlyphSortType {
	NONE, 
	FRONT_TO_BACK, 
	BACK_TO_FRONT, 
	TEXTURE
};

//holds vertex data and texture type for single sprite
//Need this to sort by texture
class Glyph {
public:
	Glyph();

	Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color);

	Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle);

	GLuint texture;
	float depth;

	Vertex topLeft;
	Vertex topRight;
	Vertex bottomLeft;
	Vertex bottomRight;
};

//store offset into VBO
//dont need seperate VBO for each draw call
class RenderBatch {
public:
	RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) :
		offset(Offset),
		numVertices(NumVertices),
		texture(Texture)
	{
	}

	GLuint offset;
	GLuint numVertices;
	GLuint texture;
};

class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();
	//Initialize spriteBatch, call before use
	void init();

	//clear renderBatch and set sort type:
	//TEXTURE, FRONTTOBACK, BACKTOFRONT
	void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
	//Sort textures, create render batch
	void end();

	//add glyph to vector of glyphs
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);
	
	//These allow for the rotation of sprites
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle);

	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir);


	void renderBatch();

private:
	void createRenderBatches();
	void createVertexArray();
	void sortGlyphs();

	//Predicates (comparators)
	static bool compareFrontToBack(Glyph* a, Glyph* b);
	static bool compareBackToFront(Glyph* a, Glyph* b);
	static bool compareTexture(Glyph* a, Glyph* b);

	GLuint mVbo;
	GLuint mVao;
	GlyphSortType mSortType;

	//Sort smaller data size objects
	std::vector < Glyph* > mGlyphPointerVector;
	//Traversing array of contiguous data decreases thrashing (data close together)
	std::vector < Glyph > mGlyphVector;
	std::vector <RenderBatch> mRenderBatchVector;
};

}