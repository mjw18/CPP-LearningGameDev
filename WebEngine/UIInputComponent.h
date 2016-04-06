#pragma once
#include "InputManager.h"
#include "LuaEngine.h"

namespace WebEngine
{
	class UIInputComponent
	{
	public:
		UIInputComponent();
		~UIInputComponent();

		void update(InputManager& inputManager);

		void registerScreenLua(LuaEngine& LEngine)
		{
			//Register MenuScreen class
			luabind::module(LEngine.getLua())
				[
					luabind::class_<UIInputComponent>("UIInputComponent")
					.def("update", &UIInputComponent::update)
				];
		}
	private:
		
	};
}

