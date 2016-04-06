#pragma once
/*
This is the MainGame APP. This is where the magic happens.
We inherit from IMainGame interface.
*/
#include <WebEngine/IMainGame.h>
#include "GameplayScreen.h"
#include "MenuScreen.h"
class App : public WebEngine::IMainGame
{
public:
	App();
	~App();

	//Override the MainGame virtual functions
	//Called on initialization
	virtual void onInit() override;
	//Called on exitGame
	virtual void onExit() override;

	//Add screens to screen list
	virtual void addScreens() override;

private:
	//Store screens here
	std::unique_ptr<MenuScreen> m_MenuScreen = nullptr;
	std::unique_ptr<GameplayScreen> m_GameplayScreen = nullptr;
};

