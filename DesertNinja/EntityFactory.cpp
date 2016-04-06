#include "EntityFactory.h"

//Pass in main game for "globals" like InputManager
EntityFactory::EntityFactory(WebEngine::IMainGame* game) : m_Game(game)
{
}


EntityFactory::~EntityFactory()
{
}

void EntityFactory::createEntity(pugi::xml_node& entityNode)
{
	//Get entity attributes and store them
	for (pugi::xml_attribute entityAttrib = entityNode.first_attribute(); entityAttrib; entityAttrib = entityAttrib.next_attribute() )
	{
		//Do stuff with attributes
	}
	
	//Go through entity components
	for (pugi::xml_node component = entityNode.first_child(); component; component = component.next_sibling())
	{
		//Do stuff with components
	}

}