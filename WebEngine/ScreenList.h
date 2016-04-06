#pragma once
/*
Set up screens and help to switch between them.
*/
#include <vector>

namespace WebEngine {

	class IMainGame;
	class IGameScreen;

	class ScreenList
	{
	public:
		ScreenList(IMainGame* game);
		~ScreenList();

		//Add a screen to the list 
		void addScreen(IGameScreen* newScreen);
		//Set a screen to the current screen
		void setScreen(int nextScreenIndex);

		IGameScreen* getNext();
		IGameScreen* getPrevious();

		void close();

		IGameScreen* getCurrent();
		
	protected:
		IMainGame* mGame = nullptr;
		std::vector<IGameScreen*> mScreenList;
		int mCurrentScreenIndex = -1;
	};

}