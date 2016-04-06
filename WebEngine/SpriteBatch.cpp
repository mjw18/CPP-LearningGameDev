#include "SpriteBatch.h"

#include <algorithm>

namespace WebEngine {

	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color) :
		texture(Texture), depth(Depth) {

		//So that we can use emplace back with Glyph vector
		topLeft.color = color;
		topLeft.setPosition(destRect.x, destRect.y + destRect.w, depth);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x, destRect.y, depth);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w, depth);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + destRect.z, destRect.y, depth);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

	}

	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle) :
		texture(Texture), depth(Depth) {

		glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

		//Find rotated points
		glm::vec2 tr = glm::vec2(halfDims.x * cos(angle) - halfDims.y * sin(angle),
			halfDims.x * sin(angle) + halfDims.y * cos(angle));

		glm::vec2 tl = glm::vec2(-halfDims.x * cos(angle) - halfDims.y * sin(angle),
			-halfDims.x * sin(angle) + halfDims.y * cos(angle));

		glm::vec2 br = -tl + halfDims;
		glm::vec2 bl = -tr + halfDims;
		tr += halfDims;
		tl += halfDims;

		//So that we can use emplace back with Glyph vector
		topLeft.color = color;
		topLeft.setPosition(destRect.x + tl.x, destRect.y + tl.y, depth);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x + bl.x, destRect.y + bl.y, depth);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRect.x + tr.x, destRect.y + tr.y, depth);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + br.x, destRect.y + br.y, depth);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

	}

	SpriteBatch::SpriteBatch() : mVbo(0), mVao(0)
	{
	}


	SpriteBatch::~SpriteBatch()
	{
	}


	void SpriteBatch::init()
	{
		createVertexArray();
	}

	//sets up state to begin rendering
	//how do we want to sort?
	void SpriteBatch::begin(GlyphSortType sortType /* GlyphSortType::TEXTURE*/)
	{
		mSortType = sortType;
		//clear vector of old data
		mRenderBatchVector.clear();
		mGlyphVector.clear();
	}

	//sort glyphs, generate batches from glyphs 
	//decide on ost efficient drawing
	void SpriteBatch::end()
	{
		//Allocate length for pointer vector
		mGlyphPointerVector.resize(mGlyphVector.size());

		//Add pointers to Pointer vector for fast sorting
		for (int i = 0; i < mGlyphVector.size(); i++)
		{
			mGlyphPointerVector[i] = &mGlyphVector[i];
		}

		//sort vector of glyphs
		sortGlyphs();

		//createBatches
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& destRect, 
							const glm::vec4& uvRect, 
							GLuint texture, 
							float depth, 
							const ColorRGBA8& color)
	{
		//Add new Glyph to vector without making a copy
		mGlyphVector.emplace_back(destRect, uvRect, texture, depth, color);
	}

	void SpriteBatch::draw(const glm::vec4& destRect,
		const glm::vec4& uvRect,
		GLuint texture,
		float depth,
		const ColorRGBA8& color,
		float angle)
	{
		mGlyphVector.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void SpriteBatch::draw(const glm::vec4& destRect,
		const glm::vec4& uvRect,
		GLuint texture,
		float depth,
		const ColorRGBA8& color,
		const glm::vec2& dir)
	{
		//get angle of rotatioon from positive x axis
		//Atan2 to account for negative
		float angle = atan2(dir.y, dir.x);

		mGlyphVector.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(mVao);

		for (int i = 0; i < mRenderBatchVector.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, mRenderBatchVector[i].texture);
			glDrawArrays(GL_TRIANGLES, mRenderBatchVector[i].offset, mRenderBatchVector[i].numVertices);
		}

		glBindVertexArray(0);
	}


	//Loop through array of glyphs and add each to batch
	//Ne texture means new batch
	void SpriteBatch::createRenderBatches()
	{
		std::vector<Vertex> vertices;
		//save time by allocating some memory AND set size
		vertices.resize(mGlyphVector.size() * 6);

		//if there are no glyphs to add to batches then return
		if (mGlyphVector.empty())
		{
			return;
		}
		
		int offset = 0;
		int currentVertex = 0;

		//add first batch
		//pass param list, adds new instance with these initializers
		mRenderBatchVector.emplace_back(offset, 6, mGlyphPointerVector[0]->texture);
		vertices[currentVertex++] = mGlyphPointerVector[0]->topLeft;
		vertices[currentVertex++] = mGlyphPointerVector[0]->bottomLeft;
		vertices[currentVertex++] = mGlyphPointerVector[0]->bottomRight;
		vertices[currentVertex++] = mGlyphPointerVector[0]->bottomRight;
		vertices[currentVertex++] = mGlyphPointerVector[0]->topRight;
		vertices[currentVertex++] = mGlyphPointerVector[0]->topLeft;
		offset += 6;

		for (int currentGlyph = 1; currentGlyph < mGlyphPointerVector.size(); currentGlyph++)
		{
			//check if glyph can be part of this batch
			if (mGlyphPointerVector[currentGlyph]->texture != mGlyphPointerVector[currentGlyph - 1]->texture)
			{
				//make new batch if texture don't match
				mRenderBatchVector.emplace_back(offset, 6, mGlyphPointerVector[currentGlyph]->texture);
			}
			else
			{
				mRenderBatchVector.back().numVertices += 6;
			}
			//pass param list, adds new instance with these initializers
			vertices[currentVertex++] = mGlyphPointerVector[currentGlyph]->topLeft;
			vertices[currentVertex++] = mGlyphPointerVector[currentGlyph]->bottomLeft;
			vertices[currentVertex++] = mGlyphPointerVector[currentGlyph]->bottomRight;
			vertices[currentVertex++] = mGlyphPointerVector[currentGlyph]->bottomRight;
			vertices[currentVertex++] = mGlyphPointerVector[currentGlyph]->topRight;
			vertices[currentVertex++] = mGlyphPointerVector[currentGlyph]->topLeft;
			offset += 6;
		}

		//Bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, mVbo);
		//orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size()*sizeof(Vertex), vertices.data());
		//unbind VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::createVertexArray()
	{
		//generate vao if not already done
		if (mVao == 0)
		{
			glGenVertexArrays(1, &mVao);
		}
		//bind vao
		glBindVertexArray(mVao);

		//generate vbo if not done
		if (mVbo == 0)
		{
			glGenBuffers(1, &mVbo);
		}
		//bind vbo
		glBindBuffer(GL_ARRAY_BUFFER, mVbo);

		//use array attributes in vertex shader
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//This is the position attribute pointer
		//how many, how many variables in each (x and y)
		//stride is size of vertex struct, offset is bytes from beg zero for pos)
		//function wants a void pointer last param
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//color attribute pointer
		//we want to normalize color inputs
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		//Disables att arrays 
		glBindVertexArray(0);
	}

	//sort glyphs
	void SpriteBatch::sortGlyphs()
	{
		//stable retains order
		switch (mSortType)
		{
		case GlyphSortType::BACK_TO_FRONT:
			//pass function that sorts, determine comparison
			std::stable_sort(mGlyphPointerVector.begin(), mGlyphPointerVector.end(), compareBackToFront);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(mGlyphPointerVector.begin(), mGlyphPointerVector.end(), compareFrontToBack);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(mGlyphPointerVector.begin(), mGlyphPointerVector.end(), compareTexture);
			break;
		}
	}
	//Predicates (comparators)
	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b)
	{
		return (a->depth < b->depth);
	}
	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b)
	{
		return (a->depth > b->depth);
	}
	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b)
	{
		return (a->texture < b->texture);
	}
}

