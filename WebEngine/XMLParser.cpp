#include "XMLParser.h"
#include"Err.h"
#include <iostream>

namespace WebEngine {

	bool XMLParser::loadXML(std::string filePath, pugi::xml_document& xmlDoc)
	{
		pugi::xml_parse_result result = xmlDoc.load_file(filePath.c_str());

		if (result)
		{
			std::printf("Loaded XML file: %s\n", filePath.c_str());
			return true;
		}
		else
		{
			std::printf("Failed to load XML file: %s \n", filePath.c_str());
			std::cout << "Result offset: " << result.offset << std::endl;
			return false;
		} 
	}

}