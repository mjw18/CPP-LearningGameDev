#pragma once
/*
This is the Game Screen Interface.
Game screens are added to the ScreenList held by each main game.
These screens include the logic and rendering of a GameState.
This is how you would Implement a a menu and Gameplay. 
These screens point to and transition from eachother.
*/
#define SCREEN_INDEX_NO_SCREEN -1

#include <luabind/luabind.hpp>
#include "IMainGame.h"

namespace WebEngine{

	enum class ScreenState{
		NONE, 
		RUNNING, 
		EXIT, 
		CHANGENEXT, 
		CHANGEPREVIOUS
	};

	class IGameScreen
	{
		//Let screen list touch its privates
		friend class ScreenList;
	public:
		IGameScreen()
		{
			//Empty
		}
		virtual  ~IGameScreen(){
			//Empty
		}

		virtual int getNextScreenIndex() const = 0;
		virtual int getPreviousScreenIndex() const = 0;

		//Called at beginning and end of application
		virtual void init() = 0;
		virtual void close() = 0;

		//When screen is switched to
		virtual void onEntry() = 0;
		//When leaving screen
		virtual void onExit() = 0;
		
		virtual void update() = 0;
		virtual void draw() = 0;

		//Setters
		void setRunning()
		{
			mCurrentState = ScreenState::RUNNING;
		}
		
		void changeNext()
		{
			mCurrentState = ScreenState::CHANGENEXT;
		}

		void changePrevious()
		{
			mCurrentState = ScreenState::CHANGEPREVIOUS;
		}

		//Getters
		int getCurrentScreenIndex() const { return mScreenIndex; }
		ScreenState getState() const { return mCurrentState; }

		//Setters
		void setParentGame(IMainGame* game) { mGame = game; }
		void setRenderDebug(bool val) { m_renderDebug = val; }

		//Have to pass in main game pointer to access LEngine in static func, ***Find a way to change!
		static void registerIGameScreenLua(lua_State* L)
		{
			luabind::module(L)
			[
				luabind::class_<IGameScreen>("IGameScreen")
				.def("changeNext", &IGameScreen::changeNext)
				.def("changePrevious", &IGameScreen::changePrevious)
				.def("setRenderDebug", &IGameScreen::setRenderDebug)
			];
		}

	protected:
		bool m_renderDebug = false;
		ScreenState mCurrentState = ScreenState::NONE;
		IMainGame* mGame = nullptr;
		int mScreenIndex = SCREEN_INDEX_NO_SCREEN;
	};

}