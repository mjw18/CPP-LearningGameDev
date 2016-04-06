#include "Err.h"

#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>

namespace WebEngine {

	void fatalError(std::string errorString)
	{
		std::cout << errorString << std::endl;
		std::cout << "press any key to continue";
		int s;
		std::cin >> s;
		SDL_Quit();
		exit(1);
	}

}