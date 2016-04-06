#include "Zombie.h"
//To be able to use Humans
#include "Human.h"
#include <WebEngine/ResourceManager.h>

Zombie::Zombie() 
{
	//Empty
}


Zombie::~Zombie()
{
}

void Zombie::init(float speed, glm::vec2 position)
{
	mHealth = 150.0f;
	mSpeed = speed;
	mPosition = position;
	//set zombie color
	mColor.r = 255;
	mColor.g = 255;
	mColor.b = 255;
	mColor.a = 255;
	mTextureID = WebEngine::ResourceManager::getTexture("Textures/ZombieGame/zombie.png").id;

}

void Zombie::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humanVector,
	std::vector<Zombie*>& zombieVector,
	float deltaTime)
{
	//Get closest human to zombie
	Human* closestHuman = getNearestHuman(humanVector);
	//Get direction to closest human
	if (closestHuman != nullptr)
	{
		glm::vec2 distVec = closestHuman->getPosition() - mPosition;
		mDirection = glm::normalize(distVec);
		mPosition += mDirection * mSpeed * deltaTime;
	}

	//Collide xmobie with tiles
	collideWithLevel(levelData);
}

Human* Zombie::getNearestHuman(std::vector<Human*>& humanVector)
{
	Human* closestHuman = nullptr;
	float smallestDistance = 99999.9f;

	for (int i = 0; i < humanVector.size(); i++)
	{
		glm::vec2 distVec = humanVector[i]->getPosition() - mPosition; 
		float distance = glm::length(distVec);

		if (distance < smallestDistance)
		{
			smallestDistance = distance;
			closestHuman = humanVector[i];
		}
	}

	return closestHuman;
}