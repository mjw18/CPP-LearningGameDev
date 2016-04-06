#include "GameplayScreen.h"
#include <iostream>
#include <SDL/SDL.h>
#include <WebEngine/IMainGame.h>
#include <WebEngine/ResourceManager.h>
#include <ctime>
#include <random>
#include <WebEngine/XMLParser.h>

GameplayScreen::GameplayScreen(WebEngine::Window* window, WebEngine::Time* time) : mWindow(window), mTime(time)
{
}

GameplayScreen::~GameplayScreen()
{
}

int GameplayScreen::getNextScreenIndex() const
{
	return 0;
}

int GameplayScreen::getPreviousScreenIndex() const
{
	return 0;
}

void GameplayScreen::init()
{
	b2Vec2 gravity(0.0f, -50.0f);
	//create up world
	mWorld = std::make_unique<b2World>(gravity);

	//Add Ground body
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -20.0f); // units of meters
	b2Body* groundBody = mWorld->CreateBody(&groundBodyDef);
	//Shape is a polygon, fixture keeps them together
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	//Initialize shaders
	//Compile texture shader
	mTextureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	mTextureProgram.addAttribute("vertexPosition");
	mTextureProgram.addAttribute("vertexColor");
	mTextureProgram.addAttribute("vertexUV");
	mTextureProgram.linkShaders();

	mDebugRenderer.init();
	m_CameraController.init(&mCamera, mTime);
	//Initialize spritebatch
	mSpriteBatch.init();

	//Init camera
	mCamera.init(mWindow->getScreenWidth(), mWindow->getScreenHeight());
	mCamera.setScale(32.0f); //32 pixels per meter, good scale

}

void GameplayScreen::close()
{

}

void GameplayScreen::onEntry()
{

}

void GameplayScreen::onExit()
{

}

void GameplayScreen::update()
{

	glm::vec2 targetPos = glm::vec2(0, 6);
	glm::vec2 prevPos = mCamera.getPosition();
	mTime->calculateDeltaTime();

	//m_CameraController.smoothCameraMove(mCamera.getPosition(), targetPos, 1.0f);
	mCamera.setPosition(targetPos);

	mCamera.update();
	checkInput();

	if (mGame->InputManager.isKeyPressed(SDLK_m))
	{
		changeNext();
	}

	//Update the physics simulation
	mWorld->Step(1.0f / 60.0f, 6, 2);
}

void GameplayScreen::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.3f, 0.4f, 1.0f);

	mTextureProgram.use();

	//get texture uniform
	GLint textureUniform = mTextureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0); //Upload uniform data
	glActiveTexture(GL_TEXTURE0);

	//Camera matrix
	glm::mat4 projectionMatrix = mCamera.getCameraMatrix();
	GLint projectionUniform = mTextureProgram.getUniformLocation("projectionMatrix");
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	mSpriteBatch.begin();

	mSpriteBatch.end();

	mSpriteBatch.renderBatch();

	mTextureProgram.unuse();

	//Debug Rendering
	if (mRenderDebug)
	{
		//Draw camera debug
		mCamera.drawDebug(mDebugRenderer);

		//Draw circle around center of screen
		mDebugRenderer.drawCircle(mCamera.screenToWorld(glm::vec2(mWindow->getScreenWidth() / 2.0f, mWindow->getScreenHeight() / 2.0f)), WebEngine::ColorRGBA8(255, 255, 255, 255), 1.0f);
		//Draw circe around mouse pisition
		mDebugRenderer.drawCircle(mCamera.screenToWorld(glm::vec2(mGame->InputManager.getMouseCoords())), WebEngine::ColorRGBA8(255, 255, 255, 255), 1.0f);
		//mDebugRenderer.drawLine(mCamera.screenToWorld(glm::vec2(mWindow->getScreenWidth() / 2.0f, mWindow->getScreenHeight() / 2.0f)), 4.0f, WebEngine::ColorRGBA8(255, 255, 255, 255), 0.0f);

		mDebugRenderer.end();
		mDebugRenderer.render(projectionMatrix, 2.0f);
	}

	//Send projection matrix to shader
	projectionUniform = mLightProgram.getUniformLocation("projectionMatrix");
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//Additive Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	//Reset to regular alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GameplayScreen::checkInput()
{
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt))
	{
		mGame->onSDLEvent(evnt);
	}
}