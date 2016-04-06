#include <SDL/SDL.h>
#include <GL/glew.h>
#include "WebEngine.h"

namespace WebEngine {

	int init(){
		//initialize SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		//Have a draw window and a buffer windw good shit
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return 0;
	}

}