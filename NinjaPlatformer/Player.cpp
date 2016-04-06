#include "Player.h"

#include <WebEngine/ResourceManager.h>
#include <iostream>
void Player::init(b2World* world, 
				  const glm::vec2& position,
				  const glm::vec2& drawDims,
				  const glm::vec2& collisionDims,
				  WebEngine::ColorRGBA8 color)
{
	//Load Player tile sheet texture
	WebEngine::GLTexture texture = WebEngine::ResourceManager::getTexture("Assets/blue_ninja.png");
	mColor = color;
	//Set up player physics box
	mCapsule.init(world, position, collisionDims, 1.0f, 0.12f, true);
	//set draw dimensions for debug renderer
	mDrawDims = drawDims;
	//initialize tile sheet
	//Pass in the x and y dimensions of the tile
	mTexture.init("XMLData/ninja.xml", texture);
}

void Player::draw(WebEngine::SpriteBatch& spriteBatch)
{
	glm::vec4 destRect;

	b2Body* body = mCapsule.getBody();

	destRect.x = body->GetPosition().x - mDrawDims.x / 2.0f;
	destRect.y = body->GetPosition().y - mDrawDims.y / 2.0f;
	destRect.z = mDrawDims.x;
	destRect.w = mCapsule.getDimensions().y; //So that feet fit to bottom

	int tileIndex;
	//Number of tiles in the animation we will play
	int numTiles;
	float animSpeed = 0.2f;
	//Store velocity for easy access
	glm::vec2 velocity;
	velocity.x = body->GetLinearVelocity().x;
	velocity.y = body->GetLinearVelocity().y;

	//Calculate animation
	if (mOnGround)
	{
		if (mAttacking)
		{
			numTiles = 4;
			tileIndex = 12;
			if (mMoveState != PlayerMoveState::ATTACKING)
			{
				mMoveState = PlayerMoveState::ATTACKING;
				mAnimTime = 0.0f;
			}
		}
		//if speed is great enough and accelerating
		else if (abs(velocity.x) > 6.0f && (velocity.x * mDirection > 0.0f))
		{
			//Running Animation
			numTiles = 6;
			tileIndex = 1;
			//Slow animation speed to account for deceleration
			animSpeed = abs(velocity.x) * 0.025;
			//Always start animation from same time (frame 0)
			if (mMoveState != PlayerMoveState::RUNNING)
			{
				mMoveState = PlayerMoveState::RUNNING;
				mAnimTime = 0;
			}
		}
		else
		{
			//Standing 
			numTiles = 1;
			tileIndex = 11;
			mMoveState = PlayerMoveState::STANDING;
		}
	}
	else
	{
		if (mAttacking)
		{
			numTiles = 1;
			tileIndex = 9;
			//Plays kick slower so that attacks last same amount of time
			animSpeed *= 0.25;

			if (mMoveState != PlayerMoveState::ATTACKING)
			{
				mMoveState = PlayerMoveState::ATTACKING;
				mAnimTime = 0.0f;			
			}
		}
		//In the air
		else if (abs(velocity.x) > 19.0f)
		{
			numTiles = 1;
			tileIndex = 10;
			mMoveState = PlayerMoveState::IN_AIR;
		}
		else if (velocity.y <= 0)
		{
			//Falling
			numTiles = 1;
			tileIndex = 8;
			mMoveState = PlayerMoveState::IN_AIR;
		}
		else
		{
			//Rising
			numTiles = 1;
			tileIndex = 7;
			mMoveState = PlayerMoveState::IN_AIR;
		}
	}

	//Check for end of attack
	if (mAnimTime > numTiles)
	{
		mAttacking = false;
	}

	//Increment animation frame
	mAnimTime += animSpeed;

	//Apply animation
	tileIndex = tileIndex + (int)mAnimTime % numTiles;

	WebEngine::Frame frame = mTexture.getFrame(tileIndex);

	//Get uv coordinates for given tile index
	glm::vec4 uvRect = mTexture.getUVs(frame);

	//If plaer direction is left, flip sprite
	if (mDirection == -1)
	{
		uvRect.x += frame.z / (float)mTexture.texture.width;
		uvRect.z *= -1;
	}

	spriteBatch.draw(destRect, uvRect, mTexture.texture.id , 0.0f, mColor, mCapsule.getBody()->GetAngle());
}

void Player::drawDebug(WebEngine::DebugRenderer& debugRenderer)
{
	mCapsule.drawDebug(debugRenderer);
}

void Player::update(WebEngine::InputManager& inputManager)
{
	b2Body* body = mCapsule.getBody();//shortcut

	if (inputManager.isKeyDown(SDLK_a))
	{
		//Change horizontal speed by applying force to player
		body->ApplyForceToCenter(b2Vec2(-50.0, 0.0), true);
		mDirection = -1;
	}
	else if (inputManager.isKeyDown(SDLK_d)) 
	{
		body->ApplyForceToCenter(b2Vec2(50.0, 0.0), true);
		mDirection = 1;
	}
	else{
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * 0.96, body->GetLinearVelocity().y));
	}

	float MAX_SPEED = 8.0f;
	if (body->GetLinearVelocity().x < -MAX_SPEED)
	{
		body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
	}
	else if (body->GetLinearVelocity().x > MAX_SPEED)
	{
		body->SetLinearVelocity( b2Vec2( MAX_SPEED, body->GetLinearVelocity().y));
	}

	//Check for attack
	if (inputManager.isKeyPressed(SDL_BUTTON_LEFT))
	{
		mAttacking = true;
	}

	//Loop thorugh list of contact points
	mOnGround = false;
	for (b2ContactEdge* ce = body->GetContactList(); ce != nullptr; ce = ce->next)
	{
		b2Contact* c = ce->contact;
		if (c->IsTouching())
		{
			b2WorldManifold manifold;
			c->GetWorldManifold(&manifold); //Manifold is filled with data
			bool below = false;
			
			//Loop through two contact points
			for (int i = 0; i < b2_maxManifoldPoints; i++)
			{
				if (manifold.points[i].y < body->GetPosition().y - mCapsule.getDimensions().y / 2.0f + mCapsule.getDimensions().x / 2.0f + 0.01f)
				{
					below = true;
					break;
				}

			}
			if (below)
			{
				mOnGround = true;
				//We can jump
				if (inputManager.isKeyPressed(SDLK_SPACE))
				{
					body->ApplyLinearImpulse(b2Vec2(0.0f, 7.5f), b2Vec2(0.0f, 0.0f), true);
					
					break; //prevent double jump
				}
			}
		}
	}
}
