#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <vector>

#include <WebEngine/GLSLProgram.h>
#include <WebEngine/GLTexture.h>
#include <WebEngine/Window.h>
#include <WebEngine/Camera2D.h>
#include <WebEngine/SpriteBatch.h>
#include <WebEngine/InputManager.h>
#include <WebEngine/Timing.h>

#include "Projectile.h"

enum class GameState{
	PLAY, EXIT
};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	//Initialize core systems
	void initSystems();

	//Initialize shaders
	void initShaders();

	//Handle player input
	void handleInput();

	//Main gmae loop
	void gameLoop();

	//Render all game objects
	void drawGame();

	//Main gmae window attributes
	WebEngine::Window _window;
	int _screenWidth;
	int _screenHeight;

	//current gamestate
	GameState _gameState;

	//GLSL program for graphics and shader compilation
	WebEngine::GLSLProgram _colorProgram;

	//Set up world camera
	WebEngine::Camera2D _camera;

	//Manage user input
	WebEngine::InputManager _inputManager;

	//store and render sprites
	WebEngine::SpriteBatch _spriteBatch;

	//Set and limit framerate
	WebEngine::FPSLimiter _fpsLimiter;

	std::vector<Projectile> _projectiles;

	float _maxFPS;
	float _FPS;
};

class Foo
{
public:
	float doStuff(int a)
	{
		return a + 1.1;
	}
};
typedef float (Foo::*Callback)(int a);
