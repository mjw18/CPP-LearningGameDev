#pragma once
#include <glm/glm.hpp>
#include "EntityFactory.h"
#include <Box2D/Box2D.h>
#include <map>
#include <WebEngine/ICommand.h>
#include <vector>

class InputComponent;
class PhysicsComponent;

class BaseEntity
{
public:
	friend class EntityFactory;

	BaseEntity();
	~BaseEntity();

	//Use composition to add these instead of virtual?
	virtual void update() = 0;

	virtual void draw() = 0;

	//getters
	const int getID() const { return m_ID; }
	bool m_onGround = false;
	int m_direction = 1;

	InputComponent* getInputComponent() { return m_input; }
	PhysicsComponent* getPhysicsComponent() { return m_physics; }

protected:

	int m_ID;

	std::map<int, char*> m_Comp;

	int NextValidID(){ static int nextID = 0; return nextID++; }

	InputComponent* m_input = nullptr;
	PhysicsComponent* m_physics = nullptr;
};

class InputComponent
{
public:
	InputComponent(BaseEntity& owner);
	~InputComponent();

	virtual void update(WebEngine::InputManager& inputManager);

private:
	BaseEntity* m_owner;
};

class PhysicsComponent
{
public:
	PhysicsComponent(BaseEntity& owner, b2World* world, glm::vec2 pos);
	~PhysicsComponent();

	virtual void update();

	b2Body* getBody() { return m_body; }///>const ??
	const glm::vec2& getDimensions() const { return m_dimensions; }
private:
	BaseEntity* m_owner;
	glm::vec2 m_dimensions;

	std::vector<WebEngine::ICommand> commandQueue;

	//Use skeleton class/component to make this better?
	b2Body* m_body;
	b2Fixture* m_fixture;

};

class JumpCommand : public WebEngine::ICommand
{
public:
	JumpCommand() {}
	virtual ~JumpCommand() {}
	virtual void execute();
private:
};