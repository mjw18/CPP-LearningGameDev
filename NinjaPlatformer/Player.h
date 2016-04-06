#pragma once

#include <WebEngine/SpriteBatch.h>
#include <WebEngine/TileSheet.h>
#include <WebEngine/InputManager.h>
#include <SDL/SDL.h>
#include "Capsule.h"

enum class PlayerMoveState {
	STANDING, RUNNING, IN_AIR, ATTACKING
};

class Player
{
public:
	void init(b2World* world, 
			  const glm::vec2& position, 
			  const glm::vec2& drawDims,
			  const glm::vec2& collisionDims, 
			  WebEngine::ColorRGBA8 color);

	void draw(WebEngine::SpriteBatch& spriteBatch);

	void drawDebug(WebEngine::DebugRenderer& debugRenderer);

	void update(WebEngine::InputManager& inputManager);

	const Capsule& getCapsule() const { return mCapsule; }

	glm::vec2 getPosition() const
	{
		glm::vec2 pos;
		pos.x = mCapsule.getBody()->GetPosition().x;
		pos.y = mCapsule.getBody()->GetPosition().y;
		return pos;
	}

	glm::vec2 getVelocity() const {
		glm::vec2 velocity;
		velocity.x = mCapsule.getBody()->GetLinearVelocity().x;
		velocity.y = mCapsule.getBody()->GetLinearVelocity().y;
		return velocity;
	}
	
	int getDirection() const
	{
		return mDirection;
	}

private:
	glm::vec2 mDrawDims;
	WebEngine::TileSheet mTexture;
	WebEngine::ColorRGBA8 mColor;
	Capsule mCapsule;

	//Player animation time. Consider getting uniform time for all event animations
	//Add this to an Animation handler perhaps
	float mAnimTime = 0;
	bool mOnGround = false;
	bool mAttacking = false;

	int mDirection = 1; //Positive 1 is right, -1 is left look

	PlayerMoveState mMoveState = PlayerMoveState::STANDING;

};

