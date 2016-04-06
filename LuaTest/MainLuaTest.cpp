
/*#include "lua.hpp"

#include <iostream>
int dmain()
{
	//Create state
	lua_State* luaVM = lua_open();

	//Enaable access to the standard libraries
	luaopen_base(luaVM);
	luaopen_string(luaVM);
	luaopen_table(luaVM);
	luaopen_math(luaVM);
	luaopen_io(luaVM);

	luaL_dofile(luaVM, "luaScript.lua");

	int a;
	std::cin >> a;

	lua_close(luaVM);
}*/