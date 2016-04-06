#include "Capsule.h"


Capsule::Capsule()
{
	//Empty
}


Capsule::~Capsule()
{
	//Empty
}

void Capsule::init(b2World* world,
	const glm::vec2& position,
	const glm::vec2& dimensions,
	float density,
	float friction,
	bool fixedRotation /*= false*/)
{
	mDimensions = dimensions;
	//Make the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = fixedRotation;
	mBody = world->CreateBody(&bodyDef);

	//Make fixture and shape
	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x / 2.0f, (dimensions.y - dimensions.x) / 2.0f);

	//Create first fixture (main body rectangle)
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density; //Can't have  nonzero density for dynamic objects
	fixtureDef.friction = friction;
	mFixtures[0] = mBody->CreateFixture(&fixtureDef);

	//Create the circles
	b2CircleShape circleShape;
	circleShape.m_radius = 0.5f * dimensions.x ;

	b2FixtureDef circleDef;
	circleDef.shape = &circleShape;
	circleDef.density = density;
	circleDef.friction = friction;

	//Bottom circle
	circleShape.m_p.Set(0.0f, (dimensions.x - dimensions.y) * 0.5f);
	mFixtures[1] = mBody->CreateFixture(&circleDef);
	//Top circle
	circleShape.m_p.Set(0.0f, (-dimensions.x + dimensions.y) * 0.5f);
	mFixtures[2] = mBody->CreateFixture(&circleDef);
}

void Capsule::drawDebug(WebEngine::DebugRenderer& debugRenderer)
{
	WebEngine::ColorRGBA8 debugColor(255, 255, 255, 255);

	glm::vec4 destRect;
	//Draw capsule box
	destRect.x = mBody->GetPosition().x - mDimensions.x * 0.5f;
	destRect.y = mBody->GetPosition().y - (mDimensions.y - mDimensions.x) * 0.5f;
	destRect.z = mDimensions.x;
	destRect.w = mDimensions.y - mDimensions.x;
	debugRenderer.drawBox(destRect, debugColor, mBody->GetAngle());

	//Draw circles
	//Draw bottom circle
	debugRenderer.drawCircle(glm::vec2(destRect.x + mDimensions.x * 0.5f, destRect.y), debugColor, mDimensions.x * 0.5f);
	debugRenderer.drawCircle(glm::vec2(destRect.x + mDimensions.x * 0.5f, destRect.y + destRect.w), debugColor, mDimensions.x * 0.5f);

}
