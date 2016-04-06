#include "Box.h"


Box::Box()
{
}


Box::~Box()
{
}

void Box::init(b2World* world,
	const glm::vec2& position,
	const glm::vec2& dimensions,
	WebEngine::GLTexture texture,
	WebEngine::ColorRGBA8 color,
	bool fixedRotation /* = false */,
	const glm::vec4& uvRect /*= glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)*/)
{
	mDimensions = dimensions;
	mColor = color;
	mTexture = texture;
	mUVRect = uvRect;
	//Make the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = fixedRotation;
	mBody = world->CreateBody(&bodyDef);

	//Make fixture and shape
	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f; //Can't have  nonzero density for dynamic objects
	fixtureDef.friction = 0.3f;
	mFixture = mBody->CreateFixture(&fixtureDef);
}

void Box::draw(WebEngine::SpriteBatch& spriteBatch)
{
	glm::vec4 destRect;
	destRect.x = mBody->GetPosition().x - mDimensions.x / 2.0f;
	destRect.y = mBody->GetPosition().y - mDimensions.y / 2.0f;
	destRect.z = mDimensions.x;
	destRect.w = mDimensions.y;

	//If using texture atlas, change UV rect
	spriteBatch.draw(destRect, mUVRect, mTexture.id, 0.0f, mColor, mBody->GetAngle());
}