
#include "Map.h"
#include "lodepng.h"
#include "defines.h"
#include <iostream>

Map::Map(float mapResolution, float robotSize):mapResolution(mapResolution), robotSize(robotSize)
{
	//robotSizeInCells  = 0.025 / 0.3 = 0.083333333....
	robotSizeInCells = robotSize / mapResolution;
	//inflationRaduis = 0.02083333333....
	inflationRadius = 0.25 * robotSizeInCells;
}

void Map::saveMapToFile(const char* filePath) {
	for (int i = 0; i < mapHeight; i++) {
			for (int j = 0; j < mapWidth; j++) {
				int c = (i * mapWidth + j) * 4;
				if (map[i][j]) {
					image[c] = 0;
					image[c + 1] = 0;
					image[c + 2] = 0;
				} else {
					image[c] = 255;
					image[c + 1] = 255;
					image[c + 2] = 255;
				}

			}
	}

	lodepng::encode(filePath, image, mapWidth, mapHeight);
}
/*
void Map::inflateMap() {

}
*/

robotBehavior Map::coarseToCordy(robotBehavior coarseGridCordy)
{
	robotBehavior cordToRet;
	cordToRet.first = coarseGridCordy.first * 2 * robotSizeInCells + robotSizeInCells;
	cordToRet.second = coarseGridCordy.second * 2 * robotSizeInCells + robotSizeInCells;
	return cordToRet;
}

robotBehavior Map::pixelToCordy(robotBehavior pixelCordy)
{
	robotBehavior coarseToRet;
	coarseToRet.first = pixelCordy.first / robotSizeInCells / 2;
	coarseToRet.second = pixelCordy.second / robotSizeInCells / 2;
	return coarseToRet;
}

realPosition Map::pixelToRobotPosition(robotBehavior pixelCordy)
{
	realPosition robotPositionToRet;
	robotPositionToRet.first = (pixelCordy.first - 0.5 * mapHeight) * mapResolution * -1;
	robotPositionToRet.second = (pixelCordy.second - 0.5 * mapWidth) * mapResolution;
	return robotPositionToRet;
}

robotBehavior Map::fineToCordy(robotBehavior fineGridCordy)
{
	robotBehavior pixelCordToRet;
	pixelCordToRet.first = (fineGridCordy.first * robotSizeInCells) + (robotSizeInCells / 2);
	pixelCordToRet.second = (fineGridCordy.second * robotSizeInCells) + (robotSizeInCells / 2);
	return pixelCordToRet;
}



void Map::loadMapFromFile(const char* filePath) {

	lodepng::decode(image, mapWidth, mapHeight, filePath);

	cout << "map size: " << mapWidth << "," << mapHeight << endl;

	map.resize(mapHeight);
	for (int i = 0; i < mapHeight; i++)
		map[i].resize(mapWidth);

	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			int c = (i * mapWidth + j) * 4;
			int r = image[c];
			int g = image[c + 1];
			int b = image[c + 2];
			if (r == 0 && g == 0 && b == 0)
			{
				map[i][j] = true;
			}
			else
			{
				map[i][j] = false;
			}
		}
	}
	vector<vector<bool> > originalMap;
		originalMap.resize(mapHeight);
		for (int i = 0; i < mapHeight; i++)
			originalMap[i].resize(mapWidth);

		for (int i = 0; i < mapHeight; i++) {
			for (int j = 0; j < mapWidth; j++) {
				originalMap[i][j] = map[i][j];
			}
		}

		for (int i = 0; i < mapHeight; i++)
		{
			for (int j = 0; j < mapWidth; j++)
			{
				if (originalMap[i][j])
				{
					for (int x = -inflationRadius+i; x<= inflationRadius+i; x++) {
						for (int y = -inflationRadius+j; y<= inflationRadius+j; y++) {
							if (x+i >= 0 && x < mapHeight && y >= 0 && y < mapWidth) {
								map[x][y] = true;
							}
						}
					}
				}
			}
		}

}

//buildFineGrid (from pixel to robotSizeInCells)
void Map::buildFineGrid() {

	int originalGridHeight = map.size();
	int originalGridWidth = map[0].size();
	int gridRows = originalGridHeight / robotSizeInCells;
	int gridCols = originalGridWidth / robotSizeInCells;
	fineGrid.resize(gridRows);
	for (int i = 0; i < gridRows; i++)
		fineGrid[i].resize(gridCols);

		for (int i = 0; i < gridRows; i++)
		{
			for (int j = 0; j < gridCols; j++)
			{
				int startedR = i * robotSizeInCells;
				int startedC = j * robotSizeInCells;
				int gridHeight = map.size();
				int gridWidth = map[0].size();
				bool flag = false;
				for (int x = startedR; x < startedR + robotSizeInCells; x++)
				{
					for (int y = startedC; y < startedC + robotSizeInCells; y++)
					{
						if (x >= gridHeight || y >= gridWidth)
							continue; // boundary check
						if (map[x][y])
						{
							flag= true;
						}
					}
				}
				fineGrid[i][j] = flag;
			}
		}

}
//buildCoarseGrid (from fineGrid to CoarseGrid)
void Map::buildCoarseGrid() {
	int originalGridHeight = fineGrid.size();
	int originalGridWidth = fineGrid[0].size();

	int gridRows = originalGridHeight / 2;
	int gridCols = originalGridWidth / 2;

	coarseGrid.resize(gridRows);
	for (int i = 0; i < gridRows; i++)
		coarseGrid[i].resize(gridCols);

		for (int i = 0; i < gridRows; i++)
		{
			for (int j = 0; j < gridCols; j++)
			{
				int startedR = i * 2;
				int startedC = j * 2;
				int gridHeight = map.size();
				int gridWidth = map[0].size();
				bool flag = false;
				for (int x = startedR; x < startedR + 2; x++)
				{
					for (int y = startedC; y < startedC + 2; y++)
					{
						if (x >= gridHeight || y >= gridWidth)
							continue;
						if (fineGrid[x][y])
						{
							flag= true;
						}
					}
				}
				coarseGrid[i][j] = flag;
			}
		}
}

Grid Map::getMapGrid() {return map;}

Grid Map::getCoarseGrid() {return coarseGrid;}

void Map::printGrid(const Grid &grid) const {
	int rows = grid.size();
	int cols = grid[0].size();
	cout << "rows: " << rows << "cols: " << cols << endl;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if(grid[i][j]==true)
			{
				cout<<"*";
			}
			else{
				cout<<" ";
			}
		}
		cout << endl;
	}
}

Map::~Map() {}

