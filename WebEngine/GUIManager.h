#pragma once
#include <SDL/SDL_events.h>

namespace WebEngine
{
	class GUIManager
	{
	public:
		GUIManager();
		~GUIManager();

		void onSDLEvent(SDL_Event& evnt)
		{
			switch (evnt.type)
			{
			case SDL_MOUSEMOTION:
				break;
			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_MOUSEBUTTONUP:
				break;
			case SDL_TEXTINPUT:
				break;
			case SDL_KEYDOWN:
				break;
			case SDL_KEYUP:
				break;
			}
		}
	private:

	};

}