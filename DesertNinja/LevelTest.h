#pragma once

#include <WebEngine/GLTexture.h>
#include <WebEngine/Spritebatch.h>
#include <WebEngine/DebugRenderer.h>
#include <Box2D/Box2D.h>
#include <string>
#include <memory>
#include <vector>

/*
Border Class

Make entity?

*/
class Border
{
public:
	Border() {};
	~Border() {};

	//Pass in array of glm::vec2 verts, improve this later
	void init(b2World* world, unsigned int count, glm::vec2* vs, float friction = 0.3);
	//Pass in vector of points
	void init(b2World* world, const std::vector<glm::vec2>& vs, float friction = 0.3);

	void drawDebug(WebEngine::DebugRenderer& debugRenderer);
	
	//Getters
	b2Body* getBody() const { return m_body; }
	b2Fixture* getFixture() const { return m_fixture; }

private:
	std::vector < glm::vec2> m_verts;
	b2Body* m_body;
	b2Fixture* m_fixture;
};

/*
Lege Class

Make entity?

*/
class Ledge
{
public:
	Ledge() {}
	~Ledge() {}

	void init(b2World* world, glm::vec2& pos, float length, float friction = 0.3, float angle = 0); ///> Given left endpoint and angle
	void init(b2World* world, glm::vec2& lEndpoint, glm::vec2& rEndpoint, float friction = 0.3); ///> Given endpoints

	void drawDebug(WebEngine::DebugRenderer& debugRenderer);

	//Getters
	b2Body* getBody() const { return m_body; }
	b2Fixture* getFixture() const { return m_fixture; }

private:
	glm::vec2 m_lEndpoint;
	glm::vec2 m_rEndpoint;

	float m_length;
	float m_angle;

	//Box 2D data
	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
};

/*

Box class

Make Entity

*/
class Box
{
public:
	Box() {};
	~Box() {};

	void init(b2World* world,
		const glm::vec2& position,
		const glm::vec2& dimensions,
		const std::string filePath,
		WebEngine::ColorRGBA8 color,
		bool dynamic = true,
		bool fixedRotation = false,
		const glm::vec4& uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0));

	//Have a differet renderer probably
	void draw(WebEngine::SpriteBatch& spriteBatch);

	void drawDebug(WebEngine::DebugRenderer& debugRenderer);

	//Getters
	b2Body* getBody() const { return m_body; }
	b2Fixture* getFixture() const { return m_fixture; }
	const glm::vec2& getDimensions() const { return m_dimensions; }
	const WebEngine::ColorRGBA8& getColor() const { return m_color; }
private:
	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	glm::vec2 m_dimensions;
	glm::vec4 m_UVRect;
	WebEngine::ColorRGBA8 m_color;
	WebEngine::GLTexture m_texture;

};

class LevelTest
{
public:
	LevelTest();
	~LevelTest();

	void init();

	//Could be updated directly and level could be container for info?
	void update();

	void draw(WebEngine::SpriteBatch& spriteBatch);

	//not sure this is necessary
	void drawDebug(WebEngine::DebugRenderer& debugRenderer);

	//Getters
	b2World* getWorld() { return m_World.get(); }
private:
	std::unique_ptr<b2World> m_World;
	glm::vec4 groundBox; 

	//Consider not using pointer
	Ledge m_Ledge;
	Ledge ledger;
	std::vector<Box> m_boxes;
	Border m_border;
};
