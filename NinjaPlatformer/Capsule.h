#pragma once

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include <WebEngine/vertex.h>
#include <WebEngine/SpriteBatch.h>
#include <WebEngine/GLTexture.h>
#include <WebEngine/DebugRenderer.h>

class Capsule
{
public:
	Capsule();
	~Capsule();

	void init(b2World* world,
		const glm::vec2& position,
		const glm::vec2& dimensions,
		float density, 
		float friction,
		bool fixedRotation = false);

	//Should put in separate layer (separate file)
	//Draw debug renderers
	void drawDebug(WebEngine::DebugRenderer& debugRenderer);

	//Getters
	b2Body* getBody() const { return mBody; }
	b2Fixture* getFixtures(int index) const { return mFixtures[index]; }
	const glm::vec2& getDimensions() const { return mDimensions; }

private:
	b2Body* mBody = nullptr;
	//Three different fixtures for hit boxes
	//THIS CAN BE USED FO COMPLEX HITBOXES
	b2Fixture* mFixtures[3];
	glm::vec2 mDimensions;
};

