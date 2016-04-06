#include "IMainGame.h"

#include "WebEngine.h"
#include "IGameScreen.h"
#include "ScreenList.h"

#include <luabind/luabind.hpp>
#include <iostream>
namespace WebEngine {

IMainGame::IMainGame()
{
	//Screen list takes a pointer to the mmain game that holds it
	mScreenList = std::make_unique<ScreenList>(this);
}


IMainGame::~IMainGame()
{
}

void IMainGame::run()
{
	if (!init()) return;

	FPSLimiter limiter;
	limiter.setMaxFPS(60.0f);
	mTime.init();

	mIsRunning = true;

	//Main game loop
	while (mIsRunning)
	{
		mTime.calculateDeltaTime();
		limiter.begin();
		//game loop
		
		InputManager.update();
		SDL_Event evnt;
		while (SDL_PollEvent(&evnt))
		{
			onSDLEvent(evnt);
		}

		update();
		draw();

		mFPS = limiter.end();
		mWindow.swapBuffer();
	}
	printf("CLOSED");
	exitGame();
}

void IMainGame::exitGame()
{
	mCurrentScreen->onExit();
	if (mScreenList)
	{
		mScreenList->close();
		//Deletes and frees smart pointer
		mScreenList.reset();
	}
	//Desstroy active window
	mWindow.close();
	
	
	mIsRunning = false;
}

bool IMainGame::init()
{
	WebEngine::init();

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	if (!initSystems()) return false;

	//Inherited, change default parameters/ additional logic
	onInit();
	addScreens();

	mCurrentScreen = mScreenList->getCurrent();
	mCurrentScreen->onEntry();
	mCurrentScreen->setRunning();

	//Add better error checking
	return true;

}

bool IMainGame::initSystems()
{
	//Fix error checking for window
	//Add function to change these parameters later
	mWindow.createWindow("Default", 1366, 768, 0);

	LEngine.init();
	//Register lua functions, new method?

	//Register Game Screen with lua
	IGameScreen::registerIGameScreenLua(LEngine.getLua());

	return true;
}

void IMainGame::onSDLEvent(SDL_Event& evnt)
{
	switch (evnt.type)
	{
	case SDL_QUIT:
		mIsRunning = false;
		exitGame();
		break;
	case SDL_MOUSEMOTION:
		InputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
		break;
	case SDL_MOUSEBUTTONDOWN:
		InputManager.pressKey(evnt.button.button);
		break;
	case SDL_MOUSEBUTTONUP:
		InputManager.releaseKey(evnt.button.button);
		break;
	case SDL_KEYDOWN:
		InputManager.pressKey(evnt.key.keysym.sym);
		break;
	case SDL_KEYUP:
		InputManager.releaseKey(evnt.key.keysym.sym);
		break;
	}
}


void IMainGame::update()
{
	if (mCurrentScreen)
	{
		switch (mCurrentScreen->getState())
		{
		case ScreenState::RUNNING:
			mCurrentScreen->update();
			break;
		case ScreenState::CHANGENEXT:
			mCurrentScreen->onExit();
			mCurrentScreen = mScreenList->getNext();
			if (mCurrentScreen)
			{
				mCurrentScreen->onEntry();
				mCurrentScreen->setRunning();
			}
			break;
		case ScreenState::CHANGEPREVIOUS:
			mCurrentScreen->onExit();
			mCurrentScreen = mScreenList->getPrevious();
			if (mCurrentScreen)
			{
				mCurrentScreen->onEntry();
				mCurrentScreen->setRunning();
			}
			break;
		case ScreenState::EXIT:
			mIsRunning = false;
			//exitGame();
			break;
		default:
			break;
		}
	}
	else
	{
		//If there is no screen, throw stupid error and ext game
		std::printf("No game screen!");
		exitGame();
	}
}

void IMainGame::draw()
{
	glViewport(0, 0, mWindow.getScreenWidth(), mWindow.getScreenHeight());

	if (mCurrentScreen && mCurrentScreen->getState() == ScreenState::RUNNING)
	{
		mCurrentScreen->draw();
	}
}

}