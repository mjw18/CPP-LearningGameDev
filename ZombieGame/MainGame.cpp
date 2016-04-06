#include "MainGame.h"

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>

#include <WebEngine/WebEngine.h>
#include <WebEngine/Timing.h>
#include <WebEngine/ResourceManager.h>
#include <glm/gtx/rotate_vector.hpp>

#include "Zombie.h"
#include "Gun.h"

const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 2.0f;
const float PLAYER_SPEED = 6.0f;

MainGame::MainGame() : 
	_screenWidth(1920),
	_screenHeight(1080),
	_gameState(GameState::PLAYING),
	_FPS(0),
	_maxFPS(60.0f),
	_player(nullptr),
	_numHumansKilled(0),
	_numZombiesKilled(0)
{
}


MainGame::~MainGame()
{
	//delete level
	for (int i = 0; i < _levelVector.size(); i++)
	{
		delete _levelVector[i];
	}
	//delete humans
	for (int i = 0; i < _humanVector.size(); i++)
	{
		delete _humanVector[i];
	}
	//delete zombies
	for (int i = 0; i < _zombieVector.size(); i++)
	{
		delete _zombieVector[i];
	}
}

void MainGame::run()
{
	//Initialize core systems
	initSystems();

	initLevels();

	WebEngine::Music music = mAudioManager.loadMusic("Sounds/06. Little Monster.ogg");
	music.fadeInMusic(2000, 1);

	//return when game is over
	gameLoop();
}

void updateFadeBloodParticle(WebEngine::Particle2D& particle, float deltaTime)
{
	particle.position += particle.velocity * deltaTime;
	particle.color.a = (GLubyte)(particle.lifeTime * 255);
}

void MainGame::initSystems()
{
	WebEngine::init();

	//Inialize sound (never before webengine)
	mAudioManager.init();

	_window.createWindow("ZOMBIE GAME", _screenWidth, _screenHeight, 0);

	//switch to grey default render color
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

	initShaders();

	//Initialize sprite batches
	_entitySpriteBatch.init();
	_hudSpriteBatch.init();

	//Initialize sprite font
	_spriteFont = new WebEngine::SpriteFont("Fonts/Hachicro.ttf", 24);

	//initialize main cameras
	_camera.init(_screenWidth, _screenHeight);

	//initialize hud camera
	_hudCamera.init(_screenWidth, _screenHeight);
	//Set camera to upper left position
	_hudCamera.setPosition(glm::vec2(_screenWidth / 2.0f, -_screenHeight / 2.0f ));

	//Initialize Particles
	mBloodParticleBatch = new WebEngine::ParticleBatch2D;
	mBloodParticleBatch->init(1000, 0.05f, 
							  WebEngine::ResourceManager::getTexture("Textures/ZombieGame/particle.png"), 
							  updateFadeBloodParticle);
	mParticleManager.addParticleBatch(mBloodParticleBatch);
}

//Initialize level and entities in it
void MainGame::initLevels()
{
	//Level 1
	_levelVector.push_back(new Level("Levels/level1.txt"));

	//Level 2
	_levelVector.push_back(new Level("Levels/level2.txt"));

	_currentLevel = 0;

	//initialize player
	_player = new Player;
	_player->init(PLAYER_SPEED, _levelVector[_currentLevel]->getPlayerPosition(), &_inputManager, &_camera, &_bulletVector);


	_humanVector.push_back(_player); //add player to human vector

	//random generator
	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));

	std::uniform_int_distribution<int>randPosX(2, (_levelVector[_currentLevel]->getWidth()-2));
	std::uniform_int_distribution<int>randPosY(2, (_levelVector[_currentLevel]->getHeight()-2));

	//Initialize randomly generated humans
	for (int i = 0; i < _levelVector[_currentLevel]->getNumHumans(); i++)
	{
		_humanVector.push_back(new Human);
		glm::vec2 humanStartPosition(randPosX(randomEngine) * TILE_WIDTH, randPosY(randomEngine) * TILE_HEIGHT);
		_humanVector.back()->init(HUMAN_SPEED, humanStartPosition);
		
	}
	//Add zombies
	//Get zombie positions
	const std::vector<glm::vec2> zombiePositions = _levelVector[_currentLevel]->getZombiePosition();

	//add all the zombies
	for (int i = 0; i < zombiePositions.size(); i++)
	{
		_zombieVector.push_back(new Zombie);
		_zombieVector.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}

	//set up player guns
	_player->addGun(new Gun("Pistol", 30, 1, 0.1f, 30.0f, 50.0f, mAudioManager.loadSoundEffect("Sounds/pistol.wav")) );
	_player->addGun(new Gun("Shotgun", 60, 10, 1.0f, 20.0f, 25.0f, mAudioManager.loadSoundEffect("Sounds/shotgun.wav")) );
	_player->addGun(new Gun("SMG", 4, 1, 0.2f, 25.0f, 35.0f, mAudioManager.loadSoundEffect("Sounds/cg1.wav")) );

}

void MainGame::initShaders()
{
	//compile shaders
	_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}

void MainGame::gameLoop()
{
	//Make sure game renders
	const float DESIRED_FPS = 60.0f; // FPS the game is designed to run at
	const int MAX_PHYSICS_STEPS = 6;//Max number of physics updates per frame
	const float MS_PER_SEC = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SEC / DESIRED_FPS; //desired miliseconds per frame;
	const float MAX_DELTA_TIME = 1.0f;//allow for time steps no greater than one

	//Use to cap FPS
	WebEngine::FPSLimiter fpsLimiter;
	fpsLimiter.init(_maxFPS*100.0f);

	//Zoom out camera
	const float CAMERA_SCALE = 1.0f / 4.0f;
	_camera.setScale(CAMERA_SCALE);

	float previousTicks = SDL_GetTicks();

	//Main game loop
	while (_gameState != GameState::EXIT)
	{
		//Start frame clock
		fpsLimiter.begin();

		//set frame time stuff
		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		checkVictory();

		handleInput();
		
		//Update with time steps
		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
		{
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

			//update physics an pass in delta time
			updateEntities(deltaTime);
			updateProjectiles(deltaTime);
			mParticleManager.update(deltaTime);

			totalDeltaTime -= deltaTime;
			i++;
		}

		//update main camera to move with player
		_camera.setPosition(_player->getPosition());
		_camera.update();

		//Update HUD camera
		_hudCamera.update();

		drawGame();

		_FPS = fpsLimiter.end();
		std::cout << _FPS << std::endl;
	}

	return;
}

void MainGame::checkVictory()
{
	//support for multiple levels
	//currnt level++
	//init level...

	//if all zombis dead we win!
	if (_zombieVector.empty())
	{
		std::cout << "Victory! \n Zombies killed: " << _numZombiesKilled << std::endl;
		std::cout << "Humans Massacred: " << _numHumansKilled << std::endl;
		std::cout << (_levelVector[_currentLevel]->getNumHumans() - 1 - _numHumansKilled - _numZombiesKilled + 2) << " survived!" << std::endl;
		int a;
		std::cin >> a;
		_gameState = GameState::EXIT;
	}
}

void MainGame::handleInput()
{
	//update inputManager
	_inputManager.update();

	SDL_Event evnt;

	static const float SCALE_SPEED = 0.03f;

	//Poll event queue until there are no more pending events
	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		}
	}
	//camera zoom and level change
	if (_inputManager.isKeyDown(SDLK_q))
	{
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}
	if (_inputManager.isKeyDown(SDLK_e))
	{
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyDown(SDLK_p))
	{
		_currentLevel = 1;
	}
}


void MainGame::updateEntities(float deltaTime)
{
	//Update all humans
	for (int i = 0; i < _humanVector.size(); i++)
	{
		_humanVector[i]->update(_levelVector[_currentLevel]->getLevelData(), _humanVector, _zombieVector, deltaTime);
	}

	//Update all zombies
	for (int i = 0; i < _zombieVector.size(); i++)
	{
		_zombieVector[i]->update(_levelVector[_currentLevel]->getLevelData(), _humanVector, _zombieVector, deltaTime);
	}

	//Update human collisions
	for (int i = 0; i < _humanVector.size(); i++)
	{
		for (int j = i + 1; j < _humanVector.size(); j++)
		{
			_humanVector[i]->collideWithEntity(_humanVector[j]);
		}
	}
	
	//Update zombie collisions
	//collide with other zombies
	for (int i = 0; i < _zombieVector.size(); i++)
	{
		for (int j = i + 1; j < _zombieVector.size(); j++)
		{
			_zombieVector[i]->collideWithEntity(_zombieVector[j]);
		}

	//collide with humans
		for (int j = i + 1 ; j < _humanVector.size(); j++)
		{
			if (_zombieVector[i]->collideWithEntity(_humanVector[j]))
			{
				addBloodParticles(_humanVector[j]->getPosition(), 15);

				//add new zombie at old human position
				_zombieVector.push_back(new Zombie);
				_zombieVector.back()->init(ZOMBIE_SPEED, _humanVector[j]->getPosition());

				//delete old human
				_humanVector[j] = _humanVector.back();
				_humanVector.pop_back();
			}
		}
		//Collide with player
		if (_zombieVector[i]->collideWithEntity(_player))
		{
			if (_player->applyDamage(5.0f)) _gameState = GameState::EXIT;
		}
	}
}

//Update Projectiles
void MainGame::updateProjectiles(float deltaTime)
{
	//collide with walls
	for (int i = 0; i < _bulletVector.size(); )
	{
		if (_bulletVector[i].update(_levelVector[_currentLevel]->getLevelData(), deltaTime))
		{
			_bulletVector[i] = _bulletVector.back();
			_bulletVector.pop_back();
		}
		else
		{
			i++;
		}
	}

	//Check to see if bullet was removed from list
	bool wasBulletRemoved = false;

	//collide with entities
	for (int i = 0; i < _bulletVector.size(); i++)
	{
		for (int j = 0; j < _zombieVector.size();)
		{
			if (_bulletVector[i].collideWithEntity(_zombieVector[j]))
			{	
				addBloodParticles(_bulletVector[i].getPosition(), 5);
				//Apply damage to the entity
				if (_zombieVector[j]->applyDamage(_bulletVector[i].getDamage()))
				{
					//If zombie died remove it
					delete _zombieVector[j];
					_zombieVector[j] = _zombieVector.back();
					_zombieVector.pop_back();
					_numZombiesKilled++;
				}
				else
				{
					j++;
				}
				//Remove bullet
				_bulletVector[i] = _bulletVector.back();
				_bulletVector.pop_back();
				wasBulletRemoved = true;
				//Since bullet is gone, no need to loop through any more zombies at this bullet
				break;
			}
			else
			{
				j++;
			}
		}
		//Collide with humans
		if (!wasBulletRemoved)
		{
			for (int j = 1; j < _humanVector.size();)
			{
				if (_bulletVector[i].collideWithEntity(_humanVector[j]))
				{
					addBloodParticles(_bulletVector[i].getPosition(), 20);
					//Apply damage to the entity
					if (_humanVector[j]->applyDamage(_bulletVector[i].getDamage()))
					{
						//If zombie died remove it
						delete _humanVector[j];
						_humanVector[j] = _humanVector.back();
						_humanVector.pop_back();
						_numHumansKilled++;
					}
					else
					{
						j++;
					}
					//Remove bullet
					_bulletVector[i] = _bulletVector.back();
					_bulletVector.pop_back();
					wasBulletRemoved = true;
					//Since bullet is gone, no need to loop through any more zombies at this bullet
					break;
				}
				else
				{
					j++;
				}
			}
		}
	}
}

void MainGame::drawGame()
{
	//Set depth to 0.0f
	glClearDepth(0.0f);

	//Clear Color and Depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//start using shaders
	_textureProgram.use();

	//set single texture
	glActiveTexture(GL_TEXTURE0);

	//Make sure shader uses texture zero
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	//Pass camera transformation matrix to shader
	GLint matrixUniform = _textureProgram.getUniformLocation("camMatrix");
	glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, &(_camera.getCameraMatrix()[0][0]));

	//Draw level
	_levelVector[_currentLevel]->drawLevel();

	//Begin drawing entities
	_entitySpriteBatch.begin();

	//get entity dimensions for culling
	glm::vec2 entityDimensions(ENTITY_WIDTH);

	//Draw humans, make new spritebatch for this
	for (int i = 0; i < _humanVector.size(); i++)
	{
		if (_camera.isBoxInView(_humanVector[i]->getPosition(), entityDimensions))
		{
			_humanVector[i]->draw(_entitySpriteBatch);
		}
	}

	//Draw zombies
	for (int i = 0; i < _zombieVector.size(); i++)
	{
		if (_camera.isBoxInView(_zombieVector[i]->getPosition(), entityDimensions))
		{
			_zombieVector[i]->draw(_entitySpriteBatch);
		}
	}

	//Draw Bullets
	for (int i = 0; i < _bulletVector.size(); i++)
	{
		_bulletVector[i].draw(_entitySpriteBatch);
	}

	//end sprite batch creation
	_entitySpriteBatch.end();

	//Render to screen
	_entitySpriteBatch.renderBatch();

	//Render particles
	mParticleManager.draw(&_entitySpriteBatch);

	//Rener HUD
	drawHUD();

	//close shaders
	_textureProgram.unuse();

	//Swap the screens
	_window.swapBuffer();
}

void MainGame::drawHUD()
{
	//Create buffer to hold string. Make it large
	char buffer[256];

	//Pass camera transformation matrix to shader
	GLint matrixUniform = _textureProgram.getUniformLocation("camMatrix");
	glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, &(_hudCamera.getCameraMatrix()[0][0]));

	_hudSpriteBatch.begin();

	//Render text
	//Read string into buffer
	sprintf_s(buffer, "Num Humans: %d", _humanVector.size());

	_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(0, -38),
		glm::vec2(1.0), 0.0f, WebEngine::ColorRGBA8(255, 255, 255, 255) );

	sprintf_s(buffer, "Num Zombies: %d", _zombieVector.size());

	_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(0, -82),
		glm::vec2(1.0), 0.0f, WebEngine::ColorRGBA8(255, 255, 255, 255));

	_hudSpriteBatch.end();

	_hudSpriteBatch.renderBatch();
}

//Draw particles
void MainGame::addBloodParticles(const glm::vec2& position, int numParticles)
{
	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);

	glm::vec2 velocity(1.5f, 0.0f);
	WebEngine::ColorRGBA8 color(255, 20, 20, 255);

	for (int i = 0; i < numParticles; i++)
	{
		mBloodParticleBatch->addParticle(position, glm::rotate(velocity, randAngle(randEngine)), color, 20.0f);
	}
}