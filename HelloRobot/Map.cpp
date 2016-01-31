
#include "Map.h"
#include "lodepng.h"
#include "defines.h"
#include <iostream>

Map::Map(float mapResolution, float robotSize) :
	mapResolution(mapResolution), robotSize(robotSize) {

	robotSizeInCells = robotSize / mapResolution;
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

void Map::inflateMap() {
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
						// bounds check
						if (x+i >= 0 && x < mapHeight && y >= 0 && y < mapWidth) {
							map[x][y] = true;
						}
					}
				}
			}
		}
	}
}


coordinatePlace Map::coarseToPixelCoordinate(coordinatePlace coarseGridCoord) {
	coordinatePlace pixelCoord;
	pixelCoord.first = coarseGridCoord.first * 2 * robotSizeInCells + robotSizeInCells;
	pixelCoord.second = coarseGridCoord.second * 2 * robotSizeInCells + robotSizeInCells;
	return pixelCoord;
}

coordinatePlace Map::pixelToCoarseCoordinate(coordinatePlace pixelCoord) {
	coordinatePlace coarseCoord;
	coarseCoord.first = pixelCoord.first / robotSizeInCells / 2;
	coarseCoord.second = pixelCoord.second / robotSizeInCells / 2;
	return coarseCoord;
}

realPosition Map::pixelToRobotPosition(coordinatePlace pixelCoord){
	realPosition robotPosition;
	robotPosition.first = (pixelCoord.first - 0.5 * mapHeight) * mapResolution * -1;
	robotPosition.second = (pixelCoord.second - 0.5 * mapWidth) * mapResolution;
	return robotPosition;
}

coordinatePlace Map::fineToPixelCoordinate(coordinatePlace fineGridCoord) {
	coordinatePlace pixelCoord;
	pixelCoord.first = fineGridCoord.first * robotSizeInCells + robotSizeInCells / 2;
	pixelCoord.second = fineGridCoord.second * robotSizeInCells + robotSizeInCells / 2;
	return pixelCoord;
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

}



void Map::reduceGrid(const Grid &originalGrid, Grid &reducedGrid, const int reduceConstant) {
	int originalGridHeight = originalGrid.size();
	int originalGridWidth = originalGrid[0].size();

	int gridRows = originalGridHeight / reduceConstant;
	int gridCols = originalGridWidth / reduceConstant;

	reducedGrid.resize(gridRows);
	for (int i = 0; i < gridRows; i++)
		reducedGrid[i].resize(gridCols);

	for (int i = 0; i < gridRows; i++)
	{
		for (int j = 0; j < gridCols; j++)
		{
			int startedR = i * reduceConstant;
			int startedC = j * reduceConstant;
			int gridHeight = originalGrid.size();
			int gridWidth = originalGrid[0].size();
			bool flag = false;
			for (int x = startedR; x < startedR + reduceConstant; x++)
			{
				for (int y = startedC; y < startedC + reduceConstant; y++)
				{
					if (x >= gridHeight || y >= gridWidth)
						continue; // boundary check
					if (originalGrid[x][y])
					{
						flag= true;
					}
				}
			}
			reducedGrid[i][j] = flag;
		}
	}

	printGrid(reducedGrid);
}


void Map::buildFineGrid() {
	reduceGrid(map, fineGrid, robotSizeInCells);
}

int Map::getCoarseGridPixelWidth() {
	return robotSizeInCells * 2;
}

int Map::getFineGridPixelWidth() {
	return robotSizeInCells;
}

Grid Map::getMapGrid() {
	return map;
}

void Map::buildCoarseGrid() {
	reduceGrid(fineGrid, coarseGrid, 2);
}

Grid Map::getCoarseGrid() {
	return coarseGrid;
}

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

Map::~Map() {
	// TODO Auto-generated destructor stub
}

