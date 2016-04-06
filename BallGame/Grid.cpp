#include "Grid.h"

Grid::Grid(int width, int height, int cellSize) :
mWidth(width),
mHeight(height),
mCellSize(cellSize)
{
	mNumCellsX = ceil((float)mWidth / mCellSize);
	mNumCellsY = ceil((float)mHeight / mCellSize);

	//Give vector correct total size
	const int BALLS_TO_RESERVE = 20;
	mCells.resize( mNumCellsY * mNumCellsX );
	for (int i = 0; i < mCells.size(); i++)
	{
		mCells[i].balls.reserve(BALLS_TO_RESERVE);
	}
}

Grid::~Grid()
{

}

void Grid::addBall( Ball* ball)
{
	Cell* cell = getCell(ball->position);
	cell->balls.push_back(ball);
	ball->ownerCell = cell;
	ball->cellMarker = cell->balls.size() - 1;
}

void Grid::addBall(Ball* ball, Cell* cell)
{
	cell->balls.push_back(ball);
	ball->ownerCell = cell;
	ball->cellMarker = cell->balls.size() - 1;
}

void Grid::removeBall(Ball* ball)
{
	std::vector<Ball*>& balls = ball->ownerCell->balls;
	//Standard vector swap
	balls[ball->cellMarker] = balls.back();
	balls.pop_back();
	//update vector index
	if (ball->cellMarker < balls.size())
	{
		balls[ball->cellMarker]->cellMarker = ball->cellMarker;
	}
	//removed ball not in vector
	ball->cellMarker = -1;
	//Removed ball points to NULL
	ball->ownerCell = nullptr;
}

Cell* Grid::getCell(int x, int y)
{
	if (x < 0) x = 0;
	if (x >= mNumCellsX) x = mNumCellsX - 1;

	if (y < 0) y = 0;
	if (y >= mNumCellsY) y = mNumCellsY - 1;

	return &mCells[y * mNumCellsX + x];
}

Cell* Grid::getCell(const glm::vec2& pos)
{
	int cellX = (int)(pos.x / mCellSize);
	int cellY = (int)(pos.y / mCellSize);

	return getCell(cellX, cellY);
}

