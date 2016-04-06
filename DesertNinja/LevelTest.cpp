#include "LevelTest.h"
#include <WebEngine/GLTexture.h>
#include <WebEngine/ResourceManager.h>

/*-----------------Border CLASS----------------------*/
void Border::init(b2World* world, unsigned int count, glm::vec2* vs, float friction /* = 0.3 */)
{
	//resize verts vector
	m_verts.resize(count);

	//allocate memory for dynamic array
	b2Vec2* verts = new b2Vec2[count];

	for (size_t i = 0; i < count; i++)
	{
		//Store vertices
		m_verts[i].x = vs[i].x;
		m_verts[i].y = vs[i].y;

		//Add vertices to b2Vec array
		verts[i].Set(vs[i].x, vs[i].y);
	}

	//Create border body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	m_body = world->CreateBody(&bodyDef);

	//Create chain shape
	b2ChainShape chain;
	chain.CreateChain(verts, count);

	//Install ghost vertices to allow chains to connet
	chain.SetPrevVertex(b2Vec2(verts[0].x - 1.0f, verts[0].y - 1.0f));
	chain.SetNextVertex(b2Vec2(verts[count - 1].x + 1.0f, verts[count - 1].y + 1.0f));

	b2FixtureDef borderDef;
	borderDef.friction = friction;
	borderDef.shape = &chain;
	m_body->CreateFixture(&borderDef);

	//delete dynamically allocated array
	delete[] verts;
	
}

//init with vector of vertices
void Border::init(b2World* world, const std::vector<glm::vec2>& vs, float friction /* = 0.3 */)
{
	size_t count = vs.size();
	//resize verts vector
	m_verts = vs;

	//allocate memory for dynamic array
	b2Vec2* verts = new b2Vec2[count];

	for (size_t i = 0; i < count; i++)
	{
		//Add vertices to b2Vec array
		verts[i].Set(vs[i].x, vs[i].y);
	}

	//Create border body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	m_body = world->CreateBody(&bodyDef);

	//Create chain shape
	b2ChainShape chain;
	chain.CreateChain(verts, count);

	//Install ghost vertices to allow chains to connet
	chain.SetPrevVertex(b2Vec2(verts[0].x - 1.0f, verts[0].y - 1.0f));
	chain.SetNextVertex(b2Vec2(verts[count - 1].x + 1.0f, verts[count - 1].y + 1.0f));

	b2FixtureDef borderDef;
	borderDef.friction = friction;
	borderDef.shape = &chain;
	m_body->CreateFixture(&borderDef);

	//delete dynamically allocated array
	delete[] verts;

}

void Border::drawDebug(WebEngine::DebugRenderer& debugRenderer)
{
	for (size_t i = 0; i < m_verts.size() - 1; i++)
	{
		debugRenderer.drawLine(m_verts[i], m_verts[i + 1], WebEngine::ColorRGBA8(255, 255, 255, 255));
	}
	return;
}

/*-----------------Ledge CLASS----------------------*/

void Ledge::init(b2World* world, glm::vec2& pos, float length, float friction /*= 0.3*/, float angle /* = 0*/)
{
	m_lEndpoint = pos;
	m_rEndpoint.x = pos.x + length / 2.0f;
	m_rEndpoint.y = pos.y + length / 2.0f;

	m_length = length;
	m_angle = angle;

	//Move this so that this body def can be used for all ledges ? 
	b2BodyDef ledgeDef;
	ledgeDef.type = b2_staticBody;
	//Create body
	m_body = world->CreateBody(&ledgeDef);

	//Create shape
	b2EdgeShape ledge;

	float halfLength = length / 2.0f;
	//b2Vec2 v1(m_midpoint.x - halfLength * cos(angle), m_midpoint.y - halfLength * sin(angle));
	//b2Vec2 v2(m_midpoint.x + halfLength*cos(angle), m_midpoint.y + halfLength*sin(angle));
	b2Vec2 v1(m_lEndpoint.x, m_lEndpoint.y);
	b2Vec2 v2(m_rEndpoint.x, m_rEndpoint.y);

	ledge.Set(v1, v2);

	b2FixtureDef fixDef;
	fixDef.shape = &ledge;
	fixDef.friction = friction;
	m_fixture = m_body->CreateFixture(&fixDef);

}

void Ledge::init(b2World* world, glm::vec2& lEndpoint, glm::vec2& rEndpoint, float friction /* = 0.3 */)
{
	m_length = sqrt((lEndpoint.x - rEndpoint.x) * (lEndpoint.x - rEndpoint.x) + (lEndpoint.y - rEndpoint.y) * (lEndpoint.y - rEndpoint.y));

	m_lEndpoint = lEndpoint;
	m_rEndpoint = rEndpoint;

	m_angle = acos(m_length / (rEndpoint.x - lEndpoint.x));

	//Move this so that this body def can be used for all ledges ? 
	b2BodyDef ledgeDef;
	ledgeDef.type = b2_staticBody;
	//ledgeDef.position.Set(pos.x, pos.y);
	//Create body
	m_body = world->CreateBody(&ledgeDef);

	//Create shape
	b2EdgeShape ledge;

	b2Vec2 v1(lEndpoint.x, lEndpoint.y);
	b2Vec2 v2(rEndpoint.x, rEndpoint.y);
	ledge.Set(v1, v2);

	b2FixtureDef fixDef;
	fixDef.shape = &ledge;
	fixDef.friction = friction;
	m_fixture = m_body->CreateFixture(&fixDef);
}

void Ledge::drawDebug(WebEngine::DebugRenderer& debugRenderer)
{
	debugRenderer.drawLine(m_lEndpoint, m_rEndpoint, WebEngine::ColorRGBA8(255, 255, 255, 255));
	return;
}

/*-----------------BOX CLASS----------------------*/

void Box::init(b2World* world,
	const glm::vec2& position,
	const glm::vec2& dimensions,
	const std::string filePath,
	WebEngine::ColorRGBA8 color,
	bool dynamic /* = true*/,
	bool fixedRotation /* = false */,
	const glm::vec4& uvRect /*= glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)*/)
{
	m_dimensions = dimensions;
	m_color = color;
	m_UVRect = uvRect;
	//Move this?
	m_texture = WebEngine::ResourceManager::getTexture(filePath);

	//Make the body ---- static definition to do it once?
	b2BodyDef bodyDef;

	//Dynamic or static body
	if (dynamic)
	{
		bodyDef.type = b2_dynamicBody;
	}
	else
	{
		bodyDef.type = b2_staticBody;
	}

	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = fixedRotation;
	m_body = world->CreateBody(&bodyDef);

	//Make fixture and shape
	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f; //Can't have  nonzero density for dynamic objects
	fixtureDef.friction = 0.3f;
	m_fixture = m_body->CreateFixture(&fixtureDef);
}

void Box::draw(WebEngine::SpriteBatch& spriteBatch)
{
	glm::vec4 destRect;
	destRect.x = m_body->GetPosition().x - m_dimensions.x / 2.0f;
	destRect.y = m_body->GetPosition().y - m_dimensions.y / 2.0f;
	destRect.z = m_dimensions.x;
	destRect.w = m_dimensions.y;

	//If using texture atlas, change UV rect
	spriteBatch.draw(destRect, m_UVRect, m_texture.id, 0.0f, m_color, m_body->GetAngle());
}

void Box::drawDebug(WebEngine::DebugRenderer& debugRenderer)
{
	glm::vec4 destRect;

	destRect.x = m_body->GetPosition().x - m_dimensions.x / 2.0f;
	destRect.y = m_body->GetPosition().y - m_dimensions.y / 2.0f;
	destRect.z = m_dimensions.x;
	destRect.w = m_dimensions.y;
	debugRenderer.drawBox(destRect, WebEngine::ColorRGBA8(255, 255, 255, 255), m_body->GetAngle());
}

/*-----------------LEVEL CLASS----------------------*/

LevelTest::LevelTest()
{
}

LevelTest::~LevelTest()
{
}

void LevelTest::init()
{
	groundBox = glm::vec4(0.0f, -20.0f, 50.0f, 10.0f);

	b2Vec2 gravity(0.0f, -50.0f);
	//create up world
	m_World = std::make_unique<b2World>(gravity);
	//Add Ground body
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -20.0f); // units of meters
	b2Body* groundBody = m_World->CreateBody(&groundBodyDef);
	//Shape is a polygon, fixture keeps them together
	b2PolygonShape groundShape;
	groundShape.SetAsBox(groundBox.z, groundBox.w);
	groundBody->CreateFixture(&groundShape, 0.0f);

	glm::vec2 ledgePos(-1.0f, -8.0f);

	//angle edge
	m_Ledge.init(m_World.get(), ledgePos, 5.0f, 0.9f, 3.1415f);

	//endpoint edge
	ledger.init(m_World.get(), glm::vec2(0.0f), glm::vec2(0.0f, -5.0f), 0.7f);

	for (int i = 0; i < 3; i++)
	{
		Box box;
		m_boxes.push_back(box);
	}

	m_boxes[0].init(m_World.get(), glm::vec2(3.0f, 1.0f), glm::vec2(4.0f, 2.0f), "Assets/bricks_top.png", WebEngine::ColorRGBA8(255, 255, 255, 255), false);
	m_boxes[1].init(m_World.get(), glm::vec2(-10.0f, 1.0f), glm::vec2(1.0f, 1.0f), "Assets/bricks_top.png", WebEngine::ColorRGBA8(120, 80, 80, 255), true);
	m_boxes[2].init(m_World.get(), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f), "Assets/bricks_top.png", WebEngine::ColorRGBA8(70, 200, 80, 255), true);



	//Vertex vector for border
	std::vector<glm::vec2> edgeVerts;
	edgeVerts.push_back(glm::vec2(-20.0f, -6.0f));
	edgeVerts.push_back(glm::vec2(-9.0f, -6.0f));
	edgeVerts.push_back(glm::vec2(-8.0f, -7.0f));
	edgeVerts.push_back(glm::vec2(-6.0f, -7.0f));

	//init border object
	m_border.init(m_World.get(), edgeVerts);

	return;
}

///>Fix this to take proper timestep
void LevelTest::update()
{
	m_World->Step((1.0f / 60.0f), 6, 2);
}

void LevelTest::draw(WebEngine::SpriteBatch& spriteBatch)
{
	for (size_t i = 0; i < m_boxes.size(); i++)
	{
		m_boxes[i].draw(spriteBatch);

	}
}

void LevelTest::drawDebug(WebEngine::DebugRenderer& debugRenderer)
{
	ledger.drawDebug(debugRenderer);
	m_Ledge.drawDebug(debugRenderer);

	for (size_t i = 0; i < m_boxes.size(); i++)
	{
		m_boxes[i].drawDebug(debugRenderer);
	}

	m_border.drawDebug(debugRenderer);
	//groundBox drawdims
	glm::vec4 destRect;
	destRect.x = groundBox.x - groundBox.z / 2.0f;
	destRect.y = groundBox.y;
	destRect.z = groundBox.z;
	destRect.w = groundBox.w;
	debugRenderer.drawBox(destRect, WebEngine::ColorRGBA8(255, 255, 255, 255), 0);
	return;
}
