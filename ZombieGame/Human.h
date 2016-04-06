#pragma once

#include <glm/glm.hpp>
#include "Entity.h"

class Human : public Entity
{
public:
	Human();
	virtual ~Human();

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies,
		float deltaTime) override;

	void init(float speed, glm::vec2 position);

private:
	int _frames;
};

