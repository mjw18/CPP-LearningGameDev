#pragma once

#include <Box2D/Box2D.h>
#include <WebEngine/IGameScreen.h>
#include <WebEngine/SpriteBatch.h>
#include <WebEngine/GLSLProgram.h>
#include <WebEngine/CameraController.h>
#include <WebEngine/Camera2D.h>
#include <WebEngine/Window.h>
#include <WebEngine/DebugRenderer.h>
#include <WebEngine/Timing.h>
#include <vector>

#include "LevelTest.h"
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

	void registerScreenLua(lua_State* L)
	{
		//Register MenuScreen class
		luabind::module(L)
			[
				luabind::class_<GameplayScreen, luabind::bases<IGameScreen> >("GameplayScreen")
			];
		//Register GameplaySceen object in globals
		luabind::globals(mGame->LEngine.getLua())["GameplayScreen"] = this;
	}

private:
	//Handle input polling
	void checkInput();

	WebEngine::Window* m_Window;
	WebEngine::Time* m_Time;
	WebEngine::GLSLProgram m_TextureProgram;
	WebEngine::SpriteBatch m_SpriteBatch;
	WebEngine::Camera2D m_Camera;
	WebEngine::CameraController m_CameraController;
	WebEngine::DebugRenderer m_DebugRenderer;
	
	LevelTest testLevel;
	Player m_player;
};

