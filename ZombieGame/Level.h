#pragma once

#include <vector>
#include <string>
#include <WebEngine/SpriteBatch.h>
#include <glm/glm.hpp>

const float TILE_WIDTH = 64.0f;
const float TILE_HEIGHT = 64.0f;

class Level
{
public:
	//Build and load level
	Level(const std::string& levelPath);
	~Level();

	void drawLevel();

	//getters
	glm::vec2 getPlayerPosition() const { return _playerStartPosition; }
	const std::vector<glm::vec2>& getZombiePosition() const { return _zombieStartPosition; } //can't change const defined vector
	const std::vector<std::string>& getLevelData() const { return _levelData; }
	int getNumHumans() const { return _numHumans; }
	int getWidth() const { return _levelData[0].size(); }
	int getHeight() const { return _levelData.size(); }
private:
	int _numHumans;
	std::vector<std::string> _levelData;
	glm::vec2 _playerStartPosition;
	std::vector<glm::vec2> _zombieStartPosition;

	WebEngine::SpriteBatch _spriteBatch;
};

