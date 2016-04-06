#pragma once
#include <map>

#include "IComponent.h"

namespace WebEngine
{
	class BaseEntity
	{
	public:
		friend class EntityFactory;

		BaseEntity();
		~BaseEntity();

		//Use composition to add these instead of virtual?
		virtual void update() = 0;

		virtual void draw() = 0;

		IComponent* getComponent(ComponentID id);

		//getters
		const int getID() const { return m_ID; }
		
	protected:

		int m_ID;

		std::map<ComponentID, IComponent*> m_Components;

		int NextValidID(){ static int nextID = 0; return nextID++; }
	};
}


