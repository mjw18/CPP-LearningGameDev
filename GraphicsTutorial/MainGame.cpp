#include "MainGame.h"
#include <WebEngine/WebEngine.h>

#include <iostream>
#include <string>
#include <WebEngine/Err.h>
#include <WebEngine/ResourceManager.h>

MainGame::MainGame() : 
	_screenWidth(1920),
	_screenHeight(1080), 
	_maxFPS(60.0f),
	_gameState(GameState::PLAY)
{
	_camera.init(_screenWidth, _screenHeight);
}

//Destructor
MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();

	//returns when game ends
	gameLoop();
}

void MainGame::initSystems()
{
	WebEngine::init();

	_window.createWindow("WebEngine", _screenWidth, _screenHeight, 0);
	initShaders();

	_fpsLimiter.init(_maxFPS);
	_spriteBatch.init();
}

void MainGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		//used for frame time rendering
		_fpsLimiter.begin();

		handleInput();

		_camera.update();

		for (size_t i = 0; i < _projectiles.size();)
		{
			if (_projectiles[i].update())
			{
				_projectiles[i] = _projectiles.back();
				_projectiles.pop_back();
			}
			else
			{
				//increment after replacing dead bullet so copy isn't skipped
				i++;
			}
		}

		drawGame();
		
		_FPS = _fpsLimiter.end();
		//print fps every 10 frames
		static int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10000)
		{
			std::cout << _FPS << std::endl;
			frameCounter = 0;
		}
	}
}

void MainGame::handleInput()
{
	SDL_Event evnt;

	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type) 
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
			break;
		}
	}

	//make enum class of key bindings instead of using SDL
	if (_inputManager.isKeyDown(SDLK_w))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (_inputManager.isKeyDown(SDLK_s))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}
	if (_inputManager.isKeyDown(SDLK_d))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}
	if (_inputManager.isKeyDown(SDLK_a))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	if (_inputManager.isKeyDown(SDLK_q))
	{
		_camera.setScale(_camera.getScale() + SCALE_SPEED);
	}
	if (_inputManager.isKeyDown(SDLK_e))
	{
		_camera.setScale(_camera.getScale() - SCALE_SPEED);
	}
	if (_inputManager.isKeyDown(SDL_BUTTON_LEFT))
	{
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;

		mouseCoords = _camera.screenToWorld(mouseCoords);
		std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;

		std::cout << _screenWidth << " " << _screenHeight << std::endl;

		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = normalize((mouseCoords - playerPosition));

		_projectiles.emplace_back(playerPosition, direction, 3.0f);

		
	}

}

void MainGame::drawGame()
{
	//gl clears to certain depth, base depth at 1.0
	glClearDepth(1.0);

	//clear screen buffer color buffer AND depth buffer (bitwise or)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_colorProgram.use();
	//no multitexturing, use first texture unit
	glActiveTexture(GL_TEXTURE0);
	//get uniform location
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	//tell shader where texture is
	glUniform1i(textureLocation, 0);

	//set orthographic cameraMatrix uniform variable
	GLuint camMatrixLocation = _colorProgram.getUniformLocation("camMatrix");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(camMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_spriteBatch.begin(WebEngine::GlyphSortType::FRONT_TO_BACK);

	glm::vec4 position(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static WebEngine::GLTexture texture = WebEngine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	WebEngine::ColorRGBA8 color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	
	//draw sprites
	_spriteBatch.draw(position, uv, texture.id, -0.5f, color);

	for (size_t i = 0; i < _projectiles.size(); i++)
	{
		_projectiles[i].draw(_spriteBatch);
	}

	_spriteBatch.end();

	_spriteBatch.renderBatch();

	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//disable the shader
	_colorProgram.unuse();

	//sat the buffer and draw everything to screen
	_window.swapBuffer();
}

