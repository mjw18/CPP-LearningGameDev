#pragma once
#include <glm/glm.hpp>
#include <WebEngine/SpriteBatch.h>

class Projectile
{
public:
	Projectile();
	Projectile(glm::vec2 position, glm::vec2 direction, float speed, int lifeTime = 300);

	~Projectile();

	void init(glm::vec2 position, glm::vec2 direction, float speed, int _lifeTime);
	void draw(WebEngine::SpriteBatch& spriteBatch);

	//returns true when lifeTime is zero
	bool update();

private:
	int _lifeTime;
	float _speed;

	glm::vec2 _position;
	glm::vec2 _direction;
};

