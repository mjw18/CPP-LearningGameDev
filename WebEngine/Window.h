#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>

namespace WebEngine {

	//window creation flags
	enum WindowFlags {
		WINDOW_HIDDEN = 0x1, WINDOW_FULLSCREEN = 0x2, WINDOW_BORDERLESS = 0x4
	};


	class Window
	{
	public:
		Window();
		~Window();

		int createWindow(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);
		//Close ad destory our window
		void close();

		void swapBuffer();

		//getters
		int getScreenWidth() { return mScreenWidth; };
		int getScreenHeight() { return mScreenHeight; };

	private:
		SDL_Window* mSDLWindow;
		int mScreenWidth = 0;
		int mScreenHeight = 0;
	};

}