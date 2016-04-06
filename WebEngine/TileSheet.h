/*
Handle the sprite sheets that will be used to animate sprites
This handles a grid of sprites
This hardcoding sucks use xml

The texture is the sprte sheet being used
The tile dimensions are the integer number of tiles in the x dimension and y dimension
//Tile numbering starts at the bottom left corner of the tile sheet (change)
Incorporate XML file use in this class (?)
*/
# pragma once

#include <glm/glm.hpp>
#include "GLTexture.h"
#include "XMLParser.h"
#include <string>
#include <iostream>
namespace WebEngine {

	struct Frame{
		Frame() : x(0), y(0), z(0), w(0){}
		Frame(int X, int Y, int Z, int W) :
			x(X), y(Y), z(Z), w(W){}
		int x, y, z, w;
	};

	class TileSheet {
	public:
		
		void init(const std::string xmlData, const GLTexture& texture)
		{
			//Use this because of naming conflict
			this->texture = texture;
			XMLParser::loadXML(xmlData, doc);
		}

		Frame getFrame(int index)
		{
			Frame frame;

			pugi::xml_node atlas = doc.first_child();

			pugi::xml_node spriteFrame = atlas.find_child_by_attribute("SubTexture", "index", std::to_string(index).c_str() /*Use itoa?*/);

			frame.z = spriteFrame.attribute("width").as_int();
			frame.w = spriteFrame.attribute("height").as_int();
			frame.x = spriteFrame.attribute("x").as_int();
			frame.y = spriteFrame.attribute("y").as_int() + frame.w;

			return frame;
		}

		//Gets the UV coords for the tile at the given index
		//This will still be useful with XML which will hold spec UV
		glm::vec4 getUVs(Frame spriteFrame)
		{

			glm::vec4 uv;
			uv.x = spriteFrame.x / (float)texture.width;
			uv.y = spriteFrame.y / (float)texture.height;
			uv.z = spriteFrame.z / (float)texture.width;
			uv.w = spriteFrame.w / (float)texture.height;

			//Return the UV coordinates for the given tile
			return uv;
		}

		pugi::xml_document doc;
		GLTexture texture;
	};

}