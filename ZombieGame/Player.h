#pragma once

#include <WebEngine/InputManager.h>
#include <WebEngine/Camera2D.h>
#include "Human.h"
#include "Level.h"
#include "Projectile.h"

//Let's us make refernces to these classes, can't use the propeties of these classes
class Gun;

class Player : public Human
{
public:
	Player();
	~Player();

	//Inject input manager to keep virtual udate overridden 
	void init(float speed, glm::vec2 position, WebEngine::InputManager* inputManager, 
				WebEngine::Camera2D* camera, std::vector<Projectile>* bullets);

	void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies,
		float deltaTime) override;

	void addGun(Gun* gun);

private:
	WebEngine::InputManager* _inputManager;
	WebEngine::Camera2D* _camera;

	std::vector<Projectile>* _bulletVector;
	std::vector<Gun*> _gunVector;
	int _currentGunIndex;
};

