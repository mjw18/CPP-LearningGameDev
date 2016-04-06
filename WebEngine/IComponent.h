/*
*  Components Derive From This interface
*/

#pragma once

namespace WebEngine 
{
	enum ComponentID
	{
		INPUT_COMPONENT,
		COLLIDER,
		RIGID_BODY_COMPONENT,
		PHYSICS_COMPONENT
	};

	class IComponent
	{
	public:
		IComponent(ComponentID id) : ID(id) {}
		virtual ~IComponent();

		ComponentID ID;
	};

}


