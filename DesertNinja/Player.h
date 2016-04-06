#pragma once
#include "BaseEntity.h"
#include <WebEngine/DebugRenderer.h>
#include <WebEngine/InputManager.h>
#include <WebEngine/Spritebatch.h>
#include <WebEngine/GLTexture.h>
#include <Box2D/Box2D.h>

class Player : public BaseEntity
{
public:
	Player();
	~Player();

	void init(b2World* world, WebEngine::InputManager& inputManager, const glm::vec2& pos, WebEngine::ColorRGBA8 color, bool fixedRotation = true);
	void close();

	virtual void update() override;
	
	virtual void draw() override;

	void debugDraw(WebEngine::DebugRenderer& debugRenderer);

	//Getters
	const glm::vec2& getPosition() const { return glm::vec2(m_body->GetPosition().x, m_body->GetPosition().y); }

	bool m_onGround = false;
	int m_direction = 1;;

	WebEngine::InputManager* m_inputManager;

	//Make component?
	WebEngine::ColorRGBA8 m_color;
	WebEngine::GLTexture m_texture;
	glm::vec2 m_dimensions;

	//Use skeleton class/component to make this better?
	b2Body* m_body;
	b2Fixture* m_fixture;

};

