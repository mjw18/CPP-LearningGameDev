#pragma once

#include <WebEngine/Window.h>
#include <WebEngine/GLSLProgram.h>
#include <WebEngine/Camera2D.h>
#include <WebEngine/InputManager.h>
#include <WebEngine/SpriteBatch.h>
#include <WebEngine/SpriteFont.h>
#include <WebEngine/AudioManager.h>
#include <WebEngine/Particle2DManager.h>
#include <WebEngine/ParticleBatch2D.h>

#include "Level.h"
#include "Player.h"
#include "Projectile.h"//Can't use forward declaration unless using pointer

class Zombie;

enum GameState {
	PLAYING, EXIT
};

class MainGame
{
public:
	MainGame();
	~MainGame();
	
	//Run the game
	void run();


private:
	//Initialiize game systems
	void initSystems();

	//Initialize game levels
	void initLevels();

	//Initialize Shaders
	void initShaders();

	//Main game loop
	void gameLoop();

	//Check for victory
	void checkVictory();

	//Handle User Input
	void handleInput();

	//Updates all entities
	void updateEntities(float deltaTime);

	//Updates all projectiles
	void updateProjectiles(float deltaTime);

	//Render Game
	void drawGame();

	//Render HUD
	void drawHUD();

	//add blood particles
	void addBloodParticles(const glm::vec2& position, int numParticles);

	//Member Variables
	int _screenWidth = 768;
	int  _screenHeight = 1024;

	//we allocate with new so Level*
	std::vector<Level*> _levelVector; //vector of levels
	std::vector<Human*> _humanVector; //vector of humans
	std::vector<Zombie*> _zombieVector;
	std::vector<Projectile> _bulletVector;
	Player* _player;
	GameState _gameState;

	WebEngine::Window _window; //Game Window
	WebEngine::SpriteBatch _entitySpriteBatch;//spriteBatch for entites
	WebEngine::GLSLProgram _textureProgram; //shader program
	WebEngine::Camera2D _camera; //main camera
	WebEngine::Camera2D _hudCamera; //HUD camera
	WebEngine::InputManager _inputManager; //Handles input
	WebEngine::AudioManager mAudioManager;
	WebEngine::Particle2DManager mParticleManager;//main Particle Manager
	WebEngine::ParticleBatch2D* mBloodParticleBatch; // batch for blood particles
	//Font sprite batch and pointer
	WebEngine::SpriteBatch _hudSpriteBatch;
	WebEngine::SpriteFont* _spriteFont;

	int _numHumansKilled; //Humans Killed by Player
	int _numZombiesKilled;

	int _currentLevel;
	float _maxFPS = 60;
	float _FPS = 0;
};

