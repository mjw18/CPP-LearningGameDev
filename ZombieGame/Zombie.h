#pragma once

#include "Entity.h"

class Zombie : public Entity
{
public:
	Zombie();
	~Zombie();

	void collideWithHuman();

	void init(float speed, glm::vec2 position);

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies,
		float deltaTime) override;

private:
	Human* getNearestHuman(std::vector<Human*>& humans);	
};

