#include "Player.h"
#include <SDL/SDL.h>
#include <WebEngine/Err.h>

Player::Player()
{
}


Player::~Player()
{
	delete m_input;
	delete m_physics;
}

void Player::init(b2World* world, WebEngine::InputManager& inputManager, const glm::vec2& pos, WebEngine::ColorRGBA8 color, bool fixedRotation /*= true*/)
{
	m_inputManager = &inputManager;

	m_color = color;
	//m_dimensions = glm::vec2(0.2f, 1.0f);

	//init with skeleton component later!!!!!!!!!!
	/*b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.fixedRotation = fixedRotation;
	m_body = world->CreateBody(&bodyDef);

	//Create shape for fixture
	b2PolygonShape playerShape;
	playerShape.SetAsBox(m_dimensions.x / 2.0f, m_dimensions.y / 2.0f); 

	//Create fixture
	b2FixtureDef playerFixDef;
	playerFixDef.shape = &playerShape;
	playerFixDef.density = 1.6f;
	playerFixDef.friction = 0.1f;
	
	m_fixture = m_body->CreateFixture(&playerFixDef);*/

	//make components
	m_input = new InputComponent(*this);
	m_physics = new PhysicsComponent(*this, world, pos);
}

void Player::close()
{

}

void Player::update()
{
	m_input->update(*m_inputManager);
	m_physics->update();
}

void Player::draw()
{

}

void Player::debugDraw(WebEngine::DebugRenderer& debugRenderer)
{
	glm::vec4 destRect;

	destRect.x = m_physics->getBody()->GetPosition().x - m_physics->getDimensions().x / 2.0f;
	destRect.y = m_physics->getBody()->GetPosition().y - m_physics->getDimensions().y / 2.0f;
	destRect.z = m_physics->getDimensions().x;
	destRect.w = m_physics->getDimensions().y;
	debugRenderer.drawBox(destRect, WebEngine::ColorRGBA8(255, 255, 255, 255), m_physics->getBody()->GetAngle());
}


