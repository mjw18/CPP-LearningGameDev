#pragma once

#include "pugixml.hpp"

namespace WebEngine{

	class XMLParser
	{
	public:

		//Returns pugi::parse_result which has last read character on error
		static bool loadXML(std::string filePath, pugi::xml_document& xmlDoc);


	};

}