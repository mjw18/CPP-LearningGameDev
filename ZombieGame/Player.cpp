#include "Player.h"
#include <SDL/SDL.h>
#include <WebEngine/ResourceManager.h>

//Have to include these files to use them even though forward declared in class
#include "Gun.h"

Player::Player() : 
	_currentGunIndex(-1),
	_bulletVector(nullptr)
{
}

Player::~Player()
{
}

void Player::init(float speed, glm::vec2 position, WebEngine::InputManager* inputManager, 
					WebEngine::Camera2D* camera, std::vector<Projectile>* bulletVector)
{
	mHealth = 150;
	mSpeed = speed;
	mPosition = position;
	_inputManager = inputManager;
	_camera = camera;
	_bulletVector = bulletVector;
	//set player color
	mColor = WebEngine::ColorRGBA8(255, 255, 255, 255);
	
	//since we use the same texture for every sprite
	mTextureID = WebEngine::ResourceManager::getTexture("Textures/ZombieGame/player.png").id;
}

void Player::addGun(Gun* gun)
{
	//Add gun to inventory
	_gunVector.push_back(gun);

	//if no gun equipped, equip gun
	if (_currentGunIndex == -1)
	{
		_currentGunIndex = 0;
	}
}

void Player::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	float deltaTime)
{
	//Key log for player
	if (_inputManager->isKeyDown(SDLK_w))
	{
		mPosition.y += mSpeed * deltaTime;
	}
	if (_inputManager->isKeyDown(SDLK_s))
	{
		mPosition.y -= mSpeed * deltaTime;
	}
	if (_inputManager->isKeyDown(SDLK_a))
	{
		mPosition.x -= mSpeed * deltaTime;
	}
	if (_inputManager->isKeyDown(SDLK_d))
	{
		mPosition.x += mSpeed * deltaTime;
	}
	if (_inputManager->isKeyDown(SDLK_1) && _gunVector.size() >= 0)
	{
		_currentGunIndex = 0;
	}
	if (_inputManager->isKeyDown(SDLK_2) && _gunVector.size() >= 1)
	{
		_currentGunIndex = 1;
	}
	if (_inputManager->isKeyDown(SDLK_3) && _gunVector.size() >= 2)
	{
		_currentGunIndex = 2;
	}

	//get coordinates of mouse click on screen
	glm::vec2 mouseCoords = _inputManager->getMouseCoords();

	//convert to world coordinates
	mouseCoords = _camera->screenToWorld(mouseCoords);

	//Center of the player sprite
	glm::vec2 centerPosition = mPosition + glm::vec2(ENTITY_RADIUS);

	//Direction of shot
	mDirection = glm::normalize(mouseCoords - centerPosition);


	if (_currentGunIndex != -1)
	{

		_gunVector[_currentGunIndex]->update(_inputManager->isKeyDown(SDL_BUTTON_LEFT),
												centerPosition,
												mDirection,
												*_bulletVector,
												deltaTime);
	}

	collideWithLevel(levelData);
}

