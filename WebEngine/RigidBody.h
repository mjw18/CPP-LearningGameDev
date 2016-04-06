#pragma once
#include <Box2D\Box2D.h>
#include "IComponent.h"

namespace WebEngine 
{
	class RigidBody : public IComponent
	{
	public:
		RigidBody();
		~RigidBody();

	private:
		b2Body* m_body;
		
	};
}



