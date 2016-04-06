#pragma once
/*
This screen handles the Main Menu.
This screen inherits from the IGameScreen interface.
*/

#include <WebEngine/IGameScreen.h>
#include <WebEngine/SpriteBatch.h>
#include <WebEngine/GLSLProgram.h>
#include <WebEngine/CameraController.h>
#include <WebEngine/Camera2D.h>
#include <WebEngine/GLTexture.h>
#include <WebEngine/Window.h>
#include <WebEngine/DebugRenderer.h>
#include <WebEngine/Timing.h>
#include <WebEngine/BasicUI.h>
#include <vector>
#include "Box.h"

class MenuScreen : public WebEngine::IGameScreen
{
public:
	MenuScreen(WebEngine::Window* window, WebEngine::Time* time);
	~MenuScreen();

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

	void registerMenuScreenLua(lua_State* L)
	{
		//Register MenuScreen class
		luabind::module(L)
		[
			luabind::class_<MenuScreen, luabind::bases<IGameScreen> >("MenuScreen")
		];
		//Register MenuSceen object in globals
		luabind::globals(mGame->LEngine.getLua())["MenuScreen"] = this;
	}

private:

	void checkInput();

	bool m_renderDebug = true;

	std::vector <WebEngine::Button*> buttonList;
	WebEngine::Window* m_Window;
	WebEngine::Time* m_Time;
	WebEngine::GLSLProgram mTextureProgram;
	WebEngine::SpriteBatch mSpriteBatch;
	WebEngine::Camera2D m_Camera;
	WebEngine::CameraController m_CameraController;
	WebEngine::GLTexture mTexture;
	WebEngine::DebugRenderer m_debugRenderer;
	WebEngine::BasicUI m_UI;
};

