#pragma once
/*
This screen handles the core gameplay.
This screen inherits from the IGameScreen interface.
*/

#include <Box2D/Box2D.h>
#include <WebEngine/IGameScreen.h>
#include <WebEngine/SpriteBatch.h>
#include <WebEngine/GLSLProgram.h>
#include <WebEngine/CameraController.h>
#include <WebEngine/Camera2D.h>
#include <WebEngine/GLTexture.h>
#include <WebEngine/Window.h>
#include <WebEngine/DebugRenderer.h>
#include <WebEngine/Timing.h>
#include <vector>

#include "Box.h"
#include "Player.h"

class GameplayScreen : public WebEngine::IGameScreen
{
public:
	GameplayScreen(WebEngine::Window* window, WebEngine::Time* time);
	~GameplayScreen();

	virtual int getNextScreenIndex() const override;
	virtual int getPreviousScreenIndex() const override;

	//Called at beginning and end of application
	virtual void init() override;
	virtual void close() override;

	//When screen is switched to
	virtual void onEntry() override;
	//When leaving screen
	virtual void onExit() override;

	virtual void update() override;
	virtual void draw() override;

private:
	int frameCount = 0;
	int frameCount2 = 0;
	int MAX_FRAMES = 100;

	void checkInput();

	std::vector<Box> mBoxes;
	//Holds physics objects and procedures
	std::unique_ptr<b2World> mWorld;
	Player mPlayer;

	bool mRenderDebug = true;

	WebEngine::Window* mWindow;
	WebEngine::Time* mTime;
	WebEngine::GLSLProgram mTextureProgram;
	WebEngine::GLSLProgram mLightProgram;
	WebEngine::SpriteBatch mSpriteBatch;
	WebEngine::Camera2D mCamera;
	WebEngine::CameraController m_CameraController;
	WebEngine::GLTexture mTexture;
	WebEngine::DebugRenderer mDebugRenderer;
};

