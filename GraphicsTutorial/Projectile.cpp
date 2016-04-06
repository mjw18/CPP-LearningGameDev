#include "Projectile.h"
#include <WebEngine/GLTexture.h>
#include <WebEngine/ResourceManager.h>


Projectile::Projectile() : 
_lifeTime(0),
_speed(0.0f),
_position(0.0f, 0.0f),
_direction(0.0f, 0.0f)
{
}

Projectile::Projectile(glm::vec2 position, glm::vec2 direction, float speed, int lifeTime)
{
	_lifeTime = lifeTime;
	_speed = speed;
	_position = position;
	_direction = direction;
}

Projectile::~Projectile()
{
}

bool Projectile::update()
{
	_position += _direction * _speed;
	_lifeTime--;
	//set to true when lifetime is 0
	if (_lifeTime <= 0)
	{
		return true;
	}

	return false;
}

void Projectile::draw(WebEngine::SpriteBatch& spriteBatch)
{
	WebEngine::ColorRGBA8 color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 posAndSize(_position, 30.0f, 30.0f);
	static WebEngine::GLTexture texture = WebEngine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	spriteBatch.draw( posAndSize , uv, texture.id, 0.2f, color);
}