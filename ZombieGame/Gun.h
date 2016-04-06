#pragma once

#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <WebEngine/AudioManager.h>

#include "Projectile.h"

class Gun
{
public:
	Gun(std::string name, int fireRate, int bulletsPerShot, float spread, 
		float bulletSpeed, float bulletDamage, WebEngine::SoundEffect fireEffect);
	~Gun();

	//Handle gun firing from mouse click
	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Projectile>& bullets, float deltaTime);

private:
	//Fire the gun
	void fire(const glm::vec2& position, const glm::vec2& direcion, std::vector<Projectile>& bullets);

	WebEngine::SoundEffect mFireEffect;
	std::string _name;
	int _fireRate; //fire rate in terms of frames
	int _bulletsPerShot; //how many bullets aare fired at a time
	float _spread; //Accuracy
	float _bulletSpeed; 
	float _bulletDamage; 
	float _frameCounter;
};

