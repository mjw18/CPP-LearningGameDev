#pragma once

#include <luabind/luabind.hpp>
#include <WebEngine/ScriptedFSM.h>

class Entity
{
public:
	Entity()
	{
		m_ID = nextValidID();
	}
	virtual ~Entity();

	//pure virtual update, add draw?
	virtual void update() = 0;
	
	//register class with lua state
	static void regiterEntityLua(lua_State* L)
	{
		luabind::module(L)
		[
			luabind::class_<Entity>("Entity")
			.def("getID", &Entity::getID)
		];
	}

	//accessors
	int getID() const { return m_ID; }
private:

	int m_ID;
	
	//used by constructor to give next valid id
	int nextValidID(){ static int NextID = 0; return NextID++; }
};

