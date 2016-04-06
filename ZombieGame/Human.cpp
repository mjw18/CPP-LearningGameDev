#include "Human.h"

#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include <WebEngine/ResourceManager.h>

Human::Human() : _frames(0)
{
}


Human::~Human()
{
}

void Human::init(float speed, glm::vec2 position)
{
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float>randDir(-1.0f, 1.0f);

	mHealth = 40.0f;
	mSpeed = speed;
	mPosition = position;
	//set human color
	mColor = WebEngine::ColorRGBA8(255, 255, 255, 255);

	mDirection = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	// Make sure direction isn't zero
	if (mDirection.length() == 0) mDirection = glm::vec2(1.0f, 0.0f);
	mDirection = glm::normalize(mDirection);
	mTextureID = WebEngine::ResourceManager::getTexture("Textures/ZombieGame/human.png").id;
}

void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	float deltaTime)
{
	static std::mt19937 randomEngine(time( nullptr ));
	static std::uniform_real_distribution<float> randAngle(-1.0f, 1.0f);

	mPosition += mDirection * mSpeed * deltaTime;

	//change direction every 30 frames
	if (_frames == 30)
	{
		mDirection = glm::rotate(mDirection, randAngle(randomEngine));
		_frames = 0;
	}
	else
	{
		_frames++;
	}

	if (collideWithLevel(levelData))
	{
		mDirection = glm::rotate(mDirection, randAngle(randomEngine));
	}
}