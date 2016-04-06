#pragma once

#include <glm/glm.hpp>
#include <string>
#include <WebEngine/SpriteBatch.h>

const float ENTITY_WIDTH = 60.0f;
const float ENTITY_RADIUS = ENTITY_WIDTH / 2;

//forward declarations prevent circular includes
class Zombie;
class Human;

struct collisionTile {
	char tile;
	glm::vec2 position;
};

class Entity
{
public:
	Entity();
	virtual  ~Entity();

	bool collideWithLevel(const std::vector<std::string>& levelData);

	virtual void update(const std::vector<std::string>& levelData, 
						std::vector<Human*>& humans,
						std::vector<Zombie*>& zombies,
						float deltaTime) = 0;

	bool collideWithEntity(Entity* entity);

	void draw(WebEngine::SpriteBatch& spriteBatch);

	//Apply damage to entity, return true if it died
	bool applyDamage(float damage);

	//setters
	void setPosition(glm::vec2 position) { mPosition = position; }
	void setTexture(std::string texturePath);
	void setHealth(float health) { mHealth = health; }

	//getters
	glm::vec2 getPosition() const { return mPosition; }
	float getHealth() const { return mHealth; }

protected:
	void checkCornerTiles(const std::vector<std::string>& levelData, 
						  std::vector<collisionTile>& collisionData);
	void collideWithTile(collisionTile tile);


	float mSpeed, mHealth;
	glm::vec2 mPosition;
	glm::vec2 mDirection = glm::vec2(1.0f, 0.0f);
	GLuint mTextureID;
	WebEngine::ColorRGBA8 mColor;
};

