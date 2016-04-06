#include "LuaEngine.h"
#include <iostream>

namespace WebEngine
{

	LuaEngine::LuaEngine()
	{
	}

	LuaEngine::~LuaEngine()
	{
		close();
	}

	void LuaEngine::init()
	{
		m_LState = luaL_newstate();
		luaL_openlibs(m_LState);

		luabind::open(m_LState);
	}

	void LuaEngine::close()
	{
		lua_close(m_LState);
	}

	//return lua state
	lua_State* LuaEngine::getLua()
	{
		return m_LState;
	}

	//Execute script file
	void LuaEngine::executeFile(const char* filePath)
	{
		if (filePath == NULL) { return; }
		
		int state = luaL_dofile(m_LState, filePath);
		luaError(state);
	}

	//Execute Lua Expression (contained in string)
	void LuaEngine::executeString(const char* expression)
	{
		if (expression == NULL) { return; }

		int state = luaL_dostring(m_LState, expression);
		luaError(state);
	}

	void LuaEngine::luaError(int state)
	{
		if (state != 0)
		{
			std::cout << "\nERR: null expression passed to script engine! " << state << std::endl;
			return;
		}

	}
}