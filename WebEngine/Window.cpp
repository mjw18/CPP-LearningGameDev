#include "Window.h"
#include "Err.h"

namespace WebEngine {

	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Window::createWindow(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
	{
		mScreenWidth = screenWidth;
		mScreenHeight = screenHeight;

		Uint32 flags = SDL_WINDOW_OPENGL;

		if (currentFlags & WINDOW_HIDDEN)
		{
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & WINDOW_FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & WINDOW_BORDERLESS)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		//Open SDL Window
		mSDLWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, 50, screenWidth, screenHeight, flags);
		if (mSDLWindow == nullptr)
		{
			fatalError("SDLWindow could not be created");
		}
		//create context window
		SDL_GLContext glContext = SDL_GL_CreateContext(mSDLWindow);
		if (glContext == nullptr)
		{
			fatalError("SDL_GL context cold not be created");
		}

		//set up glew (not essential but useful)
		GLenum error = glewInit();
		if (error != GLEW_OK)
			fatalError("glew could not init!");

		//Get OpenGL version
		std::printf("**** OpenGL version %s ****", glGetString(GL_VERSION));

		//Set background color
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		//Set VSYNC
		SDL_GL_SetSwapInterval(0);

		//Enable alpha blend
		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		//alpha encoded for PNG and multiply on source RGB color
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		//Dislpay Window info
		int i;

		// Declare display mode structure to be filled in.
		SDL_DisplayMode current;

		SDL_Init(SDL_INIT_VIDEO);

		// Get current display mode of all displays.
		for (i = 0; i < SDL_GetNumVideoDisplays(); ++i)
		{

			int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);

			if (should_be_zero != 0)
				// In case of error...
				SDL_Log("\nCould not get display mode for video display #%d: %s", i, SDL_GetError());

			else
				// On success, print the current display mode.
				SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz. %d \n", i, current.w, current.h, current.refresh_rate, current.format);
		}


		return 0;
	}

	void Window::close()
	{
		SDL_DestroyWindow(mSDLWindow);
	}

	void Window::swapBuffer()
	{
		//we set double buffer so we have to swap the buffer
		SDL_GL_SwapWindow(mSDLWindow);
	}

}