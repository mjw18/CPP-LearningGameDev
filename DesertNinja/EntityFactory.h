#pragma once
#include <WebEngine/XMLParser.h>
#include <WebEngine/IMainGame.h>

class EntityFactory
{
public:
	EntityFactory(WebEngine::IMainGame* game);
	~EntityFactory();

	void createEntity(pugi::xml_node& entityNode);

private:
	WebEngine::IMainGame* m_Game = nullptr;
};

