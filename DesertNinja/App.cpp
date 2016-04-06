#include "App.h"
#include <WebEngine/ScreenList.h>

App::App()
{
}


App::~App()
{
}

void App::onInit()
{

}

void App::onExit()
{
	return;
}

void App::addScreens()
{
	//Add gameplay screen to screen list
	m_MenuScreen = std::make_unique<MenuScreen>(&mWindow, &mTime);
	mScreenList->addScreen(m_MenuScreen.get());

	m_GameplayScreen = std::make_unique<GameplayScreen>(&mWindow, &mTime);
	mScreenList->addScreen(m_GameplayScreen.get());

	mScreenList->setScreen(m_MenuScreen->getCurrentScreenIndex());

	m_MenuScreen->registerScreenLua(LEngine.getLua()); ///>move this!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	m_GameplayScreen->registerScreenLua(LEngine.getLua());

}