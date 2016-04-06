#include "Level.h"

#include <fstream>
#include <iostream>
#include <WebEngine/Err.h>
#include <WebEngine/ResourceManager.h>
#include <WebEngine/GLTexture.h>

Level::Level(const std::string& fileName) 
{

	std::ifstream inFile;
	inFile.open(fileName);
	//Error check
	if (inFile.fail())
	{
		WebEngine::fatalError("Failed to open" + fileName);
	}

	//Throw away first string in top
	std::string temp;
	inFile >> temp >> _numHumans;
	
	std::getline(inFile, temp);

	while (std::getline(inFile, temp))
	{
		_levelData.push_back(temp);
	}

	_spriteBatch.init();
	_spriteBatch.begin();


	//Get uv rect
	glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	//Get white color
	WebEngine::ColorRGBA8 color(255, 255, 255, 255);

	//render all tiles on screen
	for (int y = 0; y < _levelData.size(); y++)
	{
		for (int x = 0; x < _levelData[y].size(); x++)
		{
			//grab tile 
			char tileData = _levelData[y][x];
			
			//get dest rect
			glm::vec4 destRect = glm::vec4(x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);

			//process tile
			switch (tileData)
			{
				case 'B':
				case 'R':				
					//draw spite with given dest rect and full uv rect
					_spriteBatch.draw(destRect, 
										uvRect, 
										WebEngine::ResourceManager::getTexture("Textures/ZombieGame/red_bricks.png").id,
										0.0f,
										color);
					break;
				case 'L':
					_spriteBatch.draw(destRect,
						uvRect,
						WebEngine::ResourceManager::getTexture("Textures/ZombieGame/light_bricks.png").id,
						0.0f,
						color);
					break;
				case 'G':
					_spriteBatch.draw(destRect,
						uvRect,
						WebEngine::ResourceManager::getTexture("Textures/ZombieGame/glass.png").id,
						0.0f,
						color);
					break;
				case '@':
					_levelData[y][x] = '.'; //prevent collision with player start
					//set starting player starting position
					_playerStartPosition.x = x * TILE_WIDTH;
					_playerStartPosition.y = y * TILE_HEIGHT; 
					break;
				case 'Z':
					_levelData[y][x] = '.'; //prevent collision with player start
					_zombieStartPosition.emplace_back(x* TILE_WIDTH, y * TILE_HEIGHT);//set zombie start position
					break;
				//do nothing for dot
				case '.':
					break;
				default:
					std::printf("Unexpected symbol %c tile at %d, %d", tileData, x, y);
					break;
			}
		}
	}

	_spriteBatch.end();
	
}

Level::~Level()
{
}

void Level::drawLevel()
{
	_spriteBatch.renderBatch();
}