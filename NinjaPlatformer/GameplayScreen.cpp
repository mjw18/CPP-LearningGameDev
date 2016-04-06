#include "GameplayScreen.h"
#include <iostream>
#include <SDL/SDL.h>
#include <WebEngine/IMainGame.h>
#include <WebEngine/ResourceManager.h>
#include <ctime>
#include <random>
#include <WebEngine/XMLParser.h>
#include "Light.h"

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

	//Load texture
	mTexture = WebEngine::ResourceManager::getTexture("Assets/bricks_top.png");

	WebEngine::ColorRGBA8 color(255, 0, 0, 255);

	//Add a bunch of boxes
	std::mt19937 randGenerator(time(nullptr));
	std::uniform_real_distribution<float> xPos(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yPos(-15.0f, 15.0f);
	std::uniform_real_distribution<float> size(0.5f, 2.5f);
	std::uniform_int_distribution<unsigned int> randColor(0, 255);
	const int numBoxes = 20;

	for (int i = 0; i < numBoxes; i++)
	{
		WebEngine::ColorRGBA8 color(randColor(randGenerator), randColor(randGenerator), randColor(randGenerator), 255);
		Box newBox;
		newBox.init(mWorld.get(), glm::vec2(xPos(randGenerator), yPos(randGenerator)), glm::vec2(size(randGenerator), size(randGenerator)), mTexture, color);
		//Box memory can be discraded because the important bits are stored in the world
		mBoxes.push_back(newBox);
	}

	//Initialize shaders
	//Compile texture shader
	mTextureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	mTextureProgram.addAttribute("vertexPosition");
	mTextureProgram.addAttribute("vertexColor");
	mTextureProgram.addAttribute("vertexUV");
	mTextureProgram.linkShaders();
	//Compile light shader
	mLightProgram.compileShaders("Shaders/lightShading.vert", "Shaders/lightShading.frag");
	mLightProgram.addAttribute("vertexPosition");
	mLightProgram.addAttribute("vertexColor");
	mLightProgram.addAttribute("vertexUV");
	mLightProgram.linkShaders();

	mDebugRenderer.init();
	m_CameraController.init(&mCamera, mTime);
	//Initialize spritebatch
	mSpriteBatch.init();

	//Init camera
	mCamera.init(mWindow->getScreenWidth(), mWindow->getScreenHeight());
	mCamera.setScale(32.0f); //32 pixels per meter, good scale

	mPlayer.init(mWorld.get(), glm::vec2(0.0f, 30.0f), glm::vec2(1.0f), glm::vec2(0.5f, 1.0f), WebEngine::ColorRGBA8(255, 255, 255, 255));
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
	
	glm::vec2 targetPos = mPlayer.getPosition() + glm::vec2(0, 6);
	glm::vec2 prevPos = mCamera.getPosition();

	//m_CameraController.smoothCameraMove(mCamera.getPosition(), targetPos, 1.0f);
	mCamera.setPosition(targetPos);
	
	mCamera.update();
	//checkInput();
	mPlayer.update(mGame->InputManager);
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

	for (auto& box : mBoxes)
	{
		box.draw(mSpriteBatch);
	}

	mPlayer.draw(mSpriteBatch);

	mSpriteBatch.end();

	mSpriteBatch.renderBatch();

	mTextureProgram.unuse();

	//Debug Rendering
	if (mRenderDebug)
	{
		glm::vec4 destRect;
		for (auto& box : mBoxes)
		{
			destRect.x = box.getBody()->GetPosition().x - box.getDimensions().x / 2.0f;
			destRect.y = box.getBody()->GetPosition().y - box.getDimensions().y / 2.0f;
			destRect.z = box.getDimensions().x;
			destRect.w = box.getDimensions().y;
			mDebugRenderer.drawBox(destRect, WebEngine::ColorRGBA8(255, 255, 255, 255), box.getBody()->GetAngle());
		}
		//Render PlayerDebug
		mPlayer.drawDebug(mDebugRenderer);

		//Draw camera debug
		mCamera.drawDebug(mDebugRenderer);

		//Draw circle around center of screen
		mDebugRenderer.drawCircle(mCamera.screenToWorld(glm::vec2(mWindow->getScreenWidth()/2.0f, mWindow->getScreenHeight() / 2.0f)), WebEngine::ColorRGBA8(255, 255, 255, 255), 1.0f);
		//Draw circe around mouse pisition
		mDebugRenderer.drawCircle(mCamera.screenToWorld(glm::vec2(mGame->InputManager.getMouseCoords())), WebEngine::ColorRGBA8(255, 255, 255, 255), 1.0f);
		//mDebugRenderer.drawLine(mCamera.screenToWorld(glm::vec2(mWindow->getScreenWidth() / 2.0f, mWindow->getScreenHeight() / 2.0f)), 4.0f, WebEngine::ColorRGBA8(255, 255, 255, 255), 0.0f);

		mDebugRenderer.end();
		mDebugRenderer.render(projectionMatrix, 2.0f);
	}

	//Render some lights
	Light playerLight;
	playerLight.color = WebEngine::ColorRGBA8(255, 255, 255, 100);
	playerLight.position = mPlayer.getPosition();
	playerLight.size = 10.0f;

	Light mouseLight;
	mouseLight.color = WebEngine::ColorRGBA8(170, 170, 30, 150);
	mouseLight.position = mCamera.screenToWorld(mGame->InputManager.getMouseCoords());
	mouseLight.size = 15.0f;

	//Bind light program
	mLightProgram.use();

	//Send projection matrix to shader
	projectionUniform = mLightProgram.getUniformLocation("projectionMatrix");
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//Additive Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	mSpriteBatch.begin();

	//Render light
	playerLight.draw(mSpriteBatch);
	mouseLight.draw(mSpriteBatch);

	mSpriteBatch.end();
	mSpriteBatch.renderBatch();

	mLightProgram.unuse();


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