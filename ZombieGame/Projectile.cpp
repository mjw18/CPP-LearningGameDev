#include "Projectile.h"

#include <WebEngine/ResourceManager.h>
#include "Entity.h"
#include "Level.h"

Projectile::Projectile(glm::vec2 position, glm::vec2 direction, float speed, float damage) :
	_speed(speed),
	_position(position),
	_direction(direction),
	_damage(damage)
{
}

Projectile::~Projectile()
{
}

//when update returns true, delete bulet
bool Projectile::update(const std::vector<std::string>& levelData, float deltaTime)
{
	_position += _direction * _speed * deltaTime;
	return collideWithLevel(levelData);
}

void Projectile::draw(WebEngine::SpriteBatch& spriteBatch)
{
	WebEngine::ColorRGBA8 color;
	color.r = 150;
	color.g = 130;
	color.b = 10;
	color.a = 255;

	//get DestRect
	glm::vec4 posAndSize = glm::vec4(_position.x + BULLET_RADIUS, 
									 _position.y + BULLET_RADIUS, 
									 BULLET_RADIUS * 2, 
									 BULLET_RADIUS * 2);
	//Get uvRect
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	//Get texture
	WebEngine::GLTexture texture = WebEngine::ResourceManager::getTexture("Textures/ZombieGame/circle.png");

	spriteBatch.draw(posAndSize, uvRect, texture.id, 0.0f, color);
}


bool Projectile::collideWithEntity(Entity* entity)
{
	const float MIN_DISTANCE = ENTITY_RADIUS + BULLET_RADIUS;

	//Get vectors for distance beteen colliding entities
	glm::vec2 centerPositionA = _position;
	glm::vec2 centerPositionB = entity->getPosition() + glm::vec2(ENTITY_RADIUS);

	//Get the distance between two entities
	glm::vec2 distVec = centerPositionA - centerPositionB;

	//Get the depth of the collision
	float collisionDepth = MIN_DISTANCE - glm::length(distVec);

	if (collisionDepth > 0)
	{
		return true;
	}
	return false;
}


bool Projectile::collideWithLevel(const std::vector<std::string>& levelData)
{
	glm::ivec2 gridPosition;
	gridPosition.x = floor(_position.x / TILE_WIDTH);
	gridPosition.y = floor(_position.y / TILE_WIDTH);

	//If bullet spawns outside of world, return
	if (gridPosition.x < 0 || gridPosition.x >= levelData[0].length() ||
		gridPosition.y < 0 || gridPosition.y >= levelData.size())
	{
		return true;
	}
	return (levelData[gridPosition.y][gridPosition.x] != '.');
}
