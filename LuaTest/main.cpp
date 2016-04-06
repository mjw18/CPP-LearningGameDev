//#include <lua/lua.h>

#pragma once
#include <WebEngine/rawInputConstants.h>
#include <SDL/SDL.h>
extern "C"
{
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

#include <iostream>
#include "luabind/luabind.hpp"
#include "testExternal.h"

void printHello()
{
	printf("\n***Hello!****\n");
}
int adder(int number)
{
	printf("\n******This doesn't actually add %d!****\n", number);
	return number;
}

class NumberPrinter
{
public:
	NumberPrinter(int number) : m_number(number)
	{}

	void print()
	{
		printf("\n---%d---\n", m_number);
	}

private:
	int m_number;
};

template<typename T> struct Point 
{
	Point(T X, T Y) : x(X), y(Y) {}

	T x, y;
};

template<typename T> struct Box
{
	Box(Point<T> uL, Point<T> lR) : UpperLeft(uL), LowerRight(lR) {}

	Point<T> UpperLeft, LowerRight;
};

void function(lua_State* L)
{
	luaL_dostring(L,
		"a = 10\n"
		"b = 3.145\n"
		"print(a+b)\n");
}

class doShit
{
public:
	doShit() {};
	~doShit() {};

	void setP( int a )
	{
		m_p = a;
	}

	void printP()
	{
		std::cout << m_p << std::endl;
	}

	int getP()
	{
		return m_p;
	}

	void setCurrentFuncTable(const luabind::object& s)
	{
		m_currentFuncTable = s;
	}

	const luabind::object&  CurrentFuncTable()const{ return m_currentFuncTable; }
private:
	luabind::object m_currentFuncTable;
	int m_p = 0;
};

int main() {
	lua_State *L = luaL_newstate();
	//luaL_openlibs(L); //open the default Lua libraries

	if (luaL_dofile(L, "firstScript.lua"))
	{
		//execute the file
		const char* err = lua_tostring(L, -1); //if there was an issue, get the string from the top of the stack...
		printf(err); // ...and print it out
	}

	//set top of stack to 0
	lua_settop(L, 0);

	//put name and age variables on the stack
	//lua_getglobal(L, "mName");
	//lua_getglobal(L, "mAge");
	lua_getglobal(L, "simple_table");

	//check that variables are of correct type
	if (!lua_istable(L, -1) )
	{
		std::cout << "\nC++ Error: Invalid type!" << std::endl;
	}
	else
	{
		lua_pushnil(L);

		while (lua_next(L, 1) != 0)
		{
			if (lua_isnumber(L, -1) || lua_isstring(L, -1) ) {
				const char* j = lua_tostring(L, -2);
				int i = (int)lua_tonumber(L, -1);
				//use number fine
				std::cout << j << "---" << i << std::endl;
			}
			lua_pop(L, 1);
		}

	
		//int mAge = (int)lua_tonumber(L, -1);
		//lua_pop(L, 1);

		//const char* mName = lua_tostring(L, -1);
		//pop top variable
	//	lua_pop(L, 1);

		//std::cout << mName << mAge << std::endl;
	}

	//Add function name to stack
	lua_getglobal(L, "add");
	if (!lua_isfunction(L, -1))
	{
		std::cout << "Error! Function not added to stack" << std::endl;
	}
	
	//push some variables onto the stack
	lua_pushnumber(L, 5);
	lua_pushnumber(L, 8);

	//Use lua_call from API to call function
	//lua_call(L, 2, 1);

	//get result from top of stack
	int result = (int)lua_tonumber(L, -1);
	std::cout << result;
	lua_pop(L, 1);

	lua_close(L);

	/****LUABIND TUTRIAL*****/
	lua_State* pL = luaL_newstate();

	luaL_openlibs(pL);

	//Connect LuaBind to this lua state
	luabind::open(pL);

	//Add function to state's global scope
	//Add multiple funcions by separating with a comma
	luabind::module(pL)
	[
		luabind::def("printHello", printHello),
		luabind::def("adder", adder),
		luabind::class_<NumberPrinter>("NumberPrinter")
		.def(luabind::constructor<int>())
		.def("print", &NumberPrinter::print),
		luabind::class_<testExternal>("TestExternal")
		.def(luabind::constructor<>())
		.def("addThings", &testExternal::addThings)
	];

	//Export class wth Luabind
	/*luabind::module(pL)
	[
		luabind::class_<NumberPrinter>("NumberPrinter")
		.def(luabind::constructor<int>())
		.def("print", &NumberPrinter::print)
	];*/

	//Export classes (structs) with luabind
	luabind::module(pL)
	[
		luabind::class_<Point<float> >("Point")
			.def(luabind::constructor<float, float>())
			.def_readwrite("X", &Point<float>::x)
			.def_readwrite("Y", &Point<float>::y),

		luabind::class_<Box<float> >("Box")
			.def(luabind::constructor< Point<float>, Point<float> >())
			
	];

	luabind::module(pL)
	[
		luabind::class_<ResourceManager>("ResourceManager")
			.def("loadResource", &ResourceManager::loadResource)
			.property("ResourceCount", &ResourceManager::getResourceCount)
	];

	//Define a lua function that we can call
	//Use c++ functions in lua
	//Use c++ classes in lua
	luaL_dostring(
		pL,
		"function add(first, second)\n"
		"a = first + second\n"
		"end\n"
		"printHello()\n"
		"adder(12)\n"
		"Tester = TestExternal()\n"
		"Tester:addThings(13, 19)\n"
		"Print2000 = NumberPrinter(2000)\n"
		"Print2000:print()\n"
	);

	std::cout << "Result: "
		<< 3
		<< std::endl;
	//luabind::call_function<void>(pL, "add", 1, 2);
	//Getter example
	try {
		ResourceManager m_ResourceManager;

		//Add m_ResourceManager to a global in lua
		luabind::globals(pL)["m_ResourceManager"] = &m_ResourceManager;

		//Execute script to load resources
		luaL_dostring(
			pL,
			"m_ResourceManager:loadResource(\"abc.txt\")\n"
			"m_REsourceManager:loadResource(\"xyz.res\")\n"
			"\n"
			"ResourceCount = m_ResourceManager.ResourceCount\n"
			);
		//Read a global from the lua script
		//size_t ResourceCount = luabind::object_cast(
			//luabind::globals(pL)["ResourceCount"]
			//);
		//std::cout << ResourceCount << std::endl;
	}
	catch (const std::exception &TheError)
	{
		std::cerr << TheError.what() << std::endl;
	}

	//function(pL);

	luabind::module(pL)
		[
			luabind::class_<doShit>("doShit")
				.def("setP", &doShit::setP)
				.def("setCurrentFuncTable", &doShit::setCurrentFuncTable)
				.def("getP", &doShit::getP)
				.def("currentFuncTable", &doShit::CurrentFuncTable)
		];


	luaL_dofile(pL, "testFuncTable.lua");

	doShit t;

	//Get global table lua object
	luabind::object state = luabind::globals(pL);

	if (luabind::type(state["testFuncTable"]) == LUA_TTABLE)
	{
		//Set t's functin tale to testFuncTable
		t.setCurrentFuncTable(state["testFuncTable"]);

		if (t.CurrentFuncTable().is_valid())
		{
			t.CurrentFuncTable()["multiply"](&t);
			std::cout << "yup" << std::endl;
		}

		std::cout << "kkkkkkkk" << t.getP() << std::endl;
	}

	t.setP(2);
	t.printP();

	lua_close(pL); //close the lua state

	getchar();
	return 0;
}