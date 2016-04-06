#pragma once

#include "Ball.h"
#include <vector>

struct Cell {
	std::vector<Ball*> balls;
};

class Grid
{
	friend class BallController;
public:
	Grid(int width, int height, int cellSize);
	~Grid();
	
	//Add a ball and determines which cell it belongs to
	void addBall(Ball* ball);
	//Adds ball to known cell
	void addBall(Ball* ball, Cell* cell);
	//Remove ball from cell in grid
	void removeBall(Ball* ball);

	//Get cell based on grid coordinates
	Cell* getCell(int x, int y);
	//Get cell based on windoe coordinates
	Cell* getCell(const glm::vec2& position);

private:
	//1D vector for cache friendliness treat as 2D
	std::vector<Cell> mCells;
	int mCellSize;
	int mWidth;
	int mHeight;
	int mNumCellsX = 0;
	int mNumCellsY = 0;
};

