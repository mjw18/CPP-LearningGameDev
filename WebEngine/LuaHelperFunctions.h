#pragma once

#include <string>
extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

namespace WebEngine {

	//Grab number from stack and pop it
	template< typename T> inline
	T luaPopNumber(lua_State* L, const char* name);

	//Grab lua string
	std::string luaPopString(lua_State* L, const char* name);

	//Get bool from Lua
	bool luaPopBool(lua_State* L, const char* name);

	template < typename T > inline
	T luaPopTableNumber(lua_State* L, const char* key);

	std::string luaPopTableString(lua_State* L, const char* key);

	bool luaPopTableBool(lua_State* L, const char* key);
}

#include "LuaHelperFunctions.hpp"