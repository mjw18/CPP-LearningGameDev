#pragma once
#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include <WebEngine/ColorRGBA8.h>
#include <WebEngine/SpriteBatch.h>
#include <WebEngine/GLTexture.h>

class Box
{
public:
	Box();
	~Box();

	void init(b2World* world, 
			  const glm::vec2& position, 
			  const glm::vec2& dimensions, 
			  WebEngine::GLTexture texture,
			  WebEngine::ColorRGBA8 color,
			  bool fixedRotation = false,
			  const glm::vec4& uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0));

	//Have a differet renderer probably
	void draw(WebEngine::SpriteBatch& spriteBatch);

	//Getters
	b2Body* getBody() const { return mBody; }
	b2Fixture* getFixture() const { return mFixture; }
	const glm::vec2& getDimensions() const { return mDimensions; }
	const WebEngine::ColorRGBA8& getColor() const { return mColor; }
private:
	b2Body* mBody = nullptr;
	b2Fixture* mFixture = nullptr;
	glm::vec2 mDimensions;
	glm::vec4 mUVRect;
	WebEngine::ColorRGBA8 mColor;
	WebEngine::GLTexture mTexture;

};

