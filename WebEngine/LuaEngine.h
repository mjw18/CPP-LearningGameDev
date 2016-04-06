#pragma once

extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include <luabind/luabind.hpp>
#include <string>
//#include "LuaHelperFunctions.h"

namespace WebEngine
{

	class LuaEngine
	{
	public:
		LuaEngine();
		~LuaEngine();

		void init();
		void close();

		//return lua state
		lua_State* getLua();

		//Execute script file
		void executeFile(const char* filePath);

		//Execute Lua Expression (contained in string)
		void executeString(const char* expression);

		//print out lua errors
		void luaError (int state);

		//int getInt(const char* name) { return luaPopNumber<int>(m_LState, name); }
	//	float getFloat(const char* name) { return luaPopNumber<float>(m_LState, name); }
		//double getDouble(const char* name) { return luaPopNumber<double>(m_LState, name); }

	//	std::string getString(const char* name){ return luaPopString(m_LState, name); }
		//bool getBool(const char* name){ return luaPopBool(m_LState, name); }

	private:
		lua_State* m_LState = nullptr;
	};

}