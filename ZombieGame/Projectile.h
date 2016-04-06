#pragma once

#include <glm/glm.hpp>
#include <string>
#include <WebEngine/GLTexture.h>
#include <WebEngine/SpriteBatch.h>

class Human;
class Zombie;
class Entity;

const float BULLET_RADIUS = 5.0f;

class Projectile
{
public:
	Projectile(glm::vec2 position, glm::vec2 direction, float speed, float damage);
	~Projectile();

	void draw(WebEngine::SpriteBatch& spriteBatch);

	bool update(const std::vector<std::string>& levelData, float deltaTime);

	bool collideWithEntity(Entity* entity);

	//getters
	float getDamage() const { return _damage; }
	glm::vec2 getPosition() const { return _position; }

private:
	//We make levelData const so that we can't alter it
	bool collideWithLevel(const std::vector<std::string>& levelData);


	glm::vec2 _position;
	glm::vec2 _direction;
	float _speed;
	float _damage;

};

