#include "Entity.h"
#include <WebEngine/GLTexture.h>
#include <WebEngine/ResourceManager.h>
#include <algorithm>

#include "Level.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}


void Entity::draw(WebEngine::SpriteBatch& spriteBatch)
{


	glm::vec4 destRect = glm::vec4(mPosition.x, mPosition.y, ENTITY_WIDTH, ENTITY_WIDTH);
	glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	spriteBatch.draw(destRect, uvRect, mTextureID, 0.0f, mColor, mDirection);
}

bool Entity::collideWithLevel(const std::vector<std::string>& levelData)
{
	std::vector<collisionTile> collisionData;

	//check tiles types of four nearest boxes
	checkCornerTiles(levelData, collisionData);

	//return if no collisions occur with alse
	if (collisionData.size() == 0)
	{
		return false;
	}
	//Do collision
	for (int i = 0; i < collisionData.size(); i++)
	{
		collideWithTile(collisionData[i]);
		//return that collsion has occcured
		return true;
	}
}

//Collide with another agent
bool Entity::collideWithEntity(Entity* entity)
{
	const float MIN_DISTANCE = ENTITY_RADIUS * 2.0f;

	//Get vectors for distance beteen colliding entities
	glm::vec2 centerPositionA = mPosition + glm::vec2(ENTITY_RADIUS);
	glm::vec2 centerPositionB = entity->getPosition() + glm::vec2(ENTITY_RADIUS);

	//Get the distance between two entities
	glm::vec2 distVec = centerPositionA - centerPositionB;
	
	//Get the depth of the collision
	float collisionDepth = MIN_DISTANCE - glm::length(distVec);

	if ( collisionDepth > 0)
	{
		//bump both agnets in direction of the distance between the agnts with half the collision depth in magnitude
		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;

		//push agnets back
		mPosition += collisionDepthVec/ 2.0f;
		entity->mPosition -= collisionDepthVec / 2.0f;

		return true;
	}
	return false;
}

void Entity::checkCornerTiles(const std::vector<std::string>& levelData, std::vector<collisionTile>& collisionData)
{
	//Check four corners of entity
	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 2; x++)
		{
			glm::vec2 cornerPos;
			cornerPos.x = floor((mPosition.x + x * ENTITY_WIDTH) / TILE_WIDTH);
			cornerPos.y = floor((mPosition.y + y * ENTITY_WIDTH) / TILE_HEIGHT);

			//If entity spawns outside of world, return
			if (cornerPos.x < 0.0f || cornerPos.x >= levelData[0].size() || 
				cornerPos.y < 0.0f || cornerPos.y >= levelData.size())
			{
				return;
			}
			//If tile is not air, collide
			if (levelData[cornerPos.y][cornerPos.x] != '.')
			{
				collisionTile tile = { levelData[cornerPos.y][cornerPos.x], ((cornerPos * TILE_WIDTH) + glm::vec2(TILE_WIDTH / 2.0f, TILE_HEIGHT / 2.0f)) };
				collisionData.push_back(tile);
			}
		}
	}
}
//sort tiles by distance before running collision to keep out of walls?
void Entity::collideWithTile(collisionTile tile)
{
	const float TILE_RADIUS = TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = ENTITY_RADIUS + TILE_RADIUS;

	glm::vec2 entityCenter = mPosition + glm::vec2(ENTITY_RADIUS);

	glm::vec2 distVec = entityCenter - tile.position;

	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	if (xDepth > 0 && yDepth > 0)
	{
		// Check which collision depth is less
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) 
		{
			// X collsion depth is smaller so we push in X direction
			if (distVec.x < 0) {
				mPosition.x -= xDepth;
			}
			else {
				mPosition.x += xDepth;
			}
		}
		else 
		{
			// Y collsion depth is smaller so we push in X direction
			if (distVec.y < 0) {
				mPosition.y -= yDepth;
			}
			else {
				mPosition.y += yDepth;
			}
		}
	}
}

//Apply damage to entity, return true if it died
bool Entity::applyDamage(float damage)
{
	mHealth -= damage;
	if (mHealth <= 0)
	{
		return true;
	}
	return false;
}