#include "BaseEntity.h"
BaseEntity::BaseEntity() : m_ID(NextValidID())
{
}


BaseEntity::~BaseEntity()
{
}

/*----------------------Input Component Shit-------------------------*/
InputComponent::InputComponent(BaseEntity& owner) : m_owner(&owner)
{

}

InputComponent::~InputComponent()
{

}

void InputComponent::update(WebEngine::InputManager& inputManager)
{
	static float dist = 0.1;

	if (inputManager.isKeyDown(SDLK_a))
	{
		//Change horizontal speed by applying force to player, greater effect if grounded (USE STATE MACHINE?!?!?! INPUT CONTEXTS????? DIFFERENT INPUT COMPONENETS????)
		if (!m_owner->m_onGround)
		{
			m_owner->getPhysicsComponent()->getBody()->ApplyForceToCenter(b2Vec2(-10.0, 0.0), true);
		}
		else
		{
			m_owner->getPhysicsComponent()->getBody()->ApplyForceToCenter(b2Vec2(-40.0, 0.0), true);
		}
		m_owner->m_direction = -1;
	}
	else if (inputManager.isKeyDown(SDLK_d))
	{
		//Change horizontal speed by applying force to player, greater effect if grounded
		if (!m_owner->m_onGround)
		{
			m_owner->getPhysicsComponent()->getBody()->ApplyForceToCenter(b2Vec2(10.0, 0.0), true);

		}
		else
		{
			m_owner->getPhysicsComponent()->getBody()->ApplyForceToCenter(b2Vec2(40.0, 0.0), true);
		}
		m_owner->m_direction = 1;
	}

	//Teleport move, make function (in lua table?) just proof of concept
	if (inputManager.isKeyDown(SDLK_s))
	{
		//Use raycast to deal with collidables
		//Use target data struct to store distance (based on button hold length ?) 
		//Move player given mag in direction of ray
		dist += 0.1;
	}
	else if (inputManager.isKeyReleased(SDLK_s))
	{
		b2Vec2 playerPos = m_owner->getPhysicsComponent()->getBody()->GetPosition();
		b2Vec2 newPos(playerPos.x + m_owner->m_direction * dist, playerPos.y);
		m_owner->getPhysicsComponent()->getBody()->SetTransform(newPos, 0.0f);
		dist = 0.0f;
	}

	//We can jump
	if (inputManager.isKeyPressed(SDLK_SPACE) && m_owner->m_onGround)
	{
		m_owner->getPhysicsComponent()->getBody()->ApplyLinearImpulse(b2Vec2(0.0f, 6.2f), b2Vec2(0.0f, 0.0f), true);
	}
}

/*-----------------Physics Component Shit------------------*/
PhysicsComponent::PhysicsComponent(BaseEntity& owner, b2World* world, glm::vec2 pos) : m_owner(&owner)
{
	m_dimensions = glm::vec2(0.2f, 1.0f);

	//init with skeleton component later!!!!!!!!!!
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.fixedRotation = true; // fixedRotation;
	m_body = world->CreateBody(&bodyDef);

	//Create shape for fixture
	b2PolygonShape playerShape;
	playerShape.SetAsBox(m_dimensions.x / 2.0f, m_dimensions.y / 2.0f);

	//Create fixture
	b2FixtureDef playerFixDef;
	playerFixDef.shape = &playerShape;
	playerFixDef.density = 1.6f;
	playerFixDef.friction = 0.1f;

	m_fixture = m_body->CreateFixture(&playerFixDef);
}

PhysicsComponent::~PhysicsComponent()
{

}

void PhysicsComponent::update()
{
	//Cap player speed
	float MAX_SPEED = 7.0f;
	if (m_body->GetLinearVelocity().x < -MAX_SPEED)
	{
		m_body->SetLinearVelocity(b2Vec2(-MAX_SPEED, m_body->GetLinearVelocity().y));
	}
	else if (m_body->GetLinearVelocity().x > MAX_SPEED)
	{
		m_body->SetLinearVelocity(b2Vec2(MAX_SPEED, m_body->GetLinearVelocity().y));
	}

	//Loop thorugh list of contact points
	m_owner->m_onGround = false;
	for (b2ContactEdge* ce = m_body->GetContactList(); ce != nullptr; ce = ce->next)
	{
		b2Contact* c = ce->contact;
		if (c->IsTouching())
		{
			b2WorldManifold manifold;
			c->GetWorldManifold(&manifold); //Manifold is filled with data
			bool below = false;

			//Loop through two contact points
			for (int i = 0; i < b2_maxManifoldPoints; i++)
			{
				if (manifold.points[i].y < m_body->GetPosition().y - m_dimensions.y / 2.0f + m_dimensions.x / 2.0f + 0.01f)
				{
					below = true;
					break;
				}

			}
			if (below)
			{
				m_owner->m_onGround = true;
			}
		}
	}
}

void JumpCommand::execute(BaseEntity& object)
{
	if ((PhysicsComponent* phys = object.getPhysicsComponent()) != nullptr)
	{
		phys->getBody()->ApplyLinearImpulse(b2Vec2(0.0f, 6.2f), b2Vec2(0.0f, 0.0f), true);

	}
}