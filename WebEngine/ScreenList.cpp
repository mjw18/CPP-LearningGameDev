#include "ScreenList.h"
#include "IGameScreen.h"

namespace WebEngine {

	ScreenList::ScreenList(IMainGame* game) : mGame(game)
	{
	}

	ScreenList::~ScreenList()
	{
		close();
	}

	//Add a screen to the list 
	void ScreenList::addScreen(IGameScreen* newScreen)
	{
		newScreen->mScreenIndex = mScreenList.size();
		//Add screen to list
		mScreenList.push_back(newScreen);
		//build new screen after adding to list
		newScreen->setParentGame(mGame);
		newScreen->init();
		
		//Set current index
		mCurrentScreenIndex = newScreen->getCurrentScreenIndex();
	}

	void ScreenList::setScreen(int nextScreenIndex)
	{
		mCurrentScreenIndex = nextScreenIndex;
	}

	IGameScreen* ScreenList::getNext()
	{
		IGameScreen* currentScreen = getCurrent();
		if (currentScreen->getNextScreenIndex() != SCREEN_INDEX_NO_SCREEN)
		{
			mCurrentScreenIndex = currentScreen->getNextScreenIndex();
		}
		return getCurrent();
	}

	IGameScreen* ScreenList::getPrevious()
	{
		IGameScreen* currentScreen = getCurrent();
		if (currentScreen->getPreviousScreenIndex() != SCREEN_INDEX_NO_SCREEN)
		{
			mCurrentScreenIndex = currentScreen->getPreviousScreenIndex();
		}
		return getCurrent();
	}

	void ScreenList::close()
	{
		for (auto& screen : mScreenList)
		{
			screen->close();
		}
		//Set the size of the list to zero
		mScreenList.resize(0);
		mCurrentScreenIndex = SCREEN_INDEX_NO_SCREEN;
	}

	IGameScreen* ScreenList::getCurrent()
	{
		if (mCurrentScreenIndex == SCREEN_INDEX_NO_SCREEN) { return nullptr; }
		return mScreenList[mCurrentScreenIndex];
	}
}