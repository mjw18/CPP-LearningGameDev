#pragma once
#include <memory>
#include "Window.h"
#include "InputManager.h"
#include "LuaEngine.h"
#include "Timing.h"

namespace WebEngine {
	
	class IGameScreen;
	class ScreenList;
	class Time;

	class IMainGame
	{
	public:
		IMainGame();
		virtual ~IMainGame();

		void run();
		void exitGame();

		void onSDLEvent(SDL_Event& evnt);

		//Additional init and exit functionality
		virtual void onInit() = 0;
		virtual void onExit() = 0;
		//Initializes game's screens
		virtual void addScreens() = 0;

		const float getFPS() const {
			return mFPS;
		}

		//Public input manager and Lua Engine
		InputManager InputManager;
		LuaEngine LEngine;
	protected:
		//Custom uodate function specific to screen
		virtual void update();
		//custom draw fnction
		virtual void draw();

		//returns false if things break
		bool init();
		bool initSystems();

		//Screen list to hold game screens
		std::unique_ptr<ScreenList> mScreenList = nullptr;
		IGameScreen* mCurrentScreen = nullptr;
		//Bool to keep track of when game quits
		bool mIsRunning = false;
		float mFPS = 0.0;
		//Game Window
		Window mWindow;
		Time mTime;
	};

}