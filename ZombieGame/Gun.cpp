#include "Gun.h"

#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>

Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletSpeed, float bulletDamage, WebEngine::SoundEffect fireEffect) : 
	_name(name),
	_fireRate(fireRate),
	_bulletsPerShot(bulletsPerShot),
	_spread(spread),
	_bulletSpeed(bulletSpeed),
	_bulletDamage(bulletDamage),
	_frameCounter(0),
	mFireEffect(fireEffect)
{
	//Empty
}


Gun::~Gun()
{
	//Empty
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Projectile>& bullets, float deltaTime)
{
	_frameCounter += 1.0f * deltaTime;
	
	if (_frameCounter >= _fireRate && isMouseDown)
	{
		fire(position, direction, bullets);
		_frameCounter = 0;
	}
}

void Gun::fire(const glm::vec2& position, const glm::vec2& direction, std::vector<Projectile>& bullets)
{
	//Initialize and seed random number generator
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randSpread(-_spread, _spread);

	//Play sound effect when gun s fired
	mFireEffect.play();

	for (int i = 0; i < _bulletsPerShot; i++)
	{
		//Add a new bullet
		bullets.emplace_back(position, 
							 glm::rotate(direction, randSpread(randomEngine)), 
							 _bulletSpeed, 
							 _bulletDamage);
	}
}