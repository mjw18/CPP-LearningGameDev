#pragma once
#include <luabind/luabind.hpp>

namespace WebEngine {

	template <class entityType>
	class ScriptedFSM
	{
	public:
		//This constructor takes pointer to the entity that owns it
		ScriptedFSM(entityType* owner) : m_owner(owner) {}
		~ScriptedFSM();

		void setCurrentState(luabind::object& newState){ m_CurrentState = newState; }

		//Call execute on valide luatable
		void updateScriptedFSM()
		{
			if (m_currentState.is_valid())
			{
				m_currrentState.at("Execute")(m_owner);
			}
		}
		
		//Change to next lua table in FSM
		void changeState(luabind::object& newState)
		{
			//Call exit methid for current state
			m_currentState.at("Exit")(m_owner);

			//Reassign current state
			m_currentState = newState;

			//call entry for new state
			m_currentState.at("Enter")(m_owner);
		}

		const luabind::object& getCurrentState() const { return m_currentState; }

		//Register ScriptedFSM class in lua
		static void registerScriptedFSMLua(lua_State* L)
		{
			luabind::module(L)
			[
				luabind::class_< ScriptedFSM<entityType> >("ScritpedFSM")
					.def("setCurrentState", &ScriptedFSM<entityType>::setCurrentState)
					.def("changeState", &ScriptedFSM<entityType>::changeState)
					.def("getCurrentState", &ScriptedFSM<entityType>::getCurrentState)
			];
		}

	private:
		//Pointer to owner entity
		entityType* m_owner;

		//Current state stored as luat table ie. luabing::object type
		luabind::object m_currentState;
	};

}