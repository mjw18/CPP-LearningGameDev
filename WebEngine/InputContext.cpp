#include "InputContext.h"
#include "XMLParser.h"

InputContext::InputContext()
{
}


InputContext::~InputContext()
{
}
/*
some func()
{

	pugi::xml_document doc;
	WebEngine::XMLParser::loadXML("XMLData/contexts.xml", doc);

	//parse result returns a bool and contains the last line that was parsed (used for error checking)
	//std::cout << "Load result: " << result.description() << ", mesh name: " << doc.child("mesh").attribute("name").value() << std::endl;

	pugi::xml_node contexts = doc.child("GameContext");
	//pugi::xml_node keys = doc.child("GameContext").child("Context");

	// tag::basic[]
	for (pugi::xml_node context = contexts.child("Context"); context; context = context.next_sibling())
	{
		std::cout << "Context:";

		for (pugi::xml_attribute attr = context.first_attribute(); attr; attr = attr.next_attribute())
		{
			std::cout << " " << attr.name() << "=" << attr.value();
		}

		for (pugi::xml_node key = context.child("Key"); key; key = key.next_sibling("Key"))
		{
			std::cout << "Type: " << key.attribute("type").value();
			std::cout << ", Name: " << key.attribute("name").value() << "\n";

		}
		std::cout << std::endl;
	}
	// end::basic[]

	std::cout << std::endl;
}
*/