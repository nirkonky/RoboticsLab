/*
 * Map.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: colman
 */

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
	/* Example: Color the first 10 rows of the image with red
	 *
	 * for (int i = 0; i < 10; i++) {
		for (int j = 0; j < mapWidth; j++) {
			int c = (i * mapWidth + j) * 4;
			image[c] = 255;
			image[c + 1] = 0;
			image[c + 2] = 0;
		}
	}*/

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

	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			if (originalMap[i][j]) {
				inflateCell(i, j);
			}
		}
	}
}

Coordinate Map::coarseToPixelCoordinate(Coordinate coarseGridCoord) {
	Coordinate pixelCoord;
	pixelCoord.first = coarseGridCoord.first * 2 * robotSizeInCells + robotSizeInCells;
	pixelCoord.second = coarseGridCoord.second * 2 * robotSizeInCells + robotSizeInCells;
	return pixelCoord;
}

Coordinate Map::pixelToCoarseCoordinate(Coordinate pixelCoord) {
	Coordinate coarseCoord;
	coarseCoord.first = pixelCoord.first / robotSizeInCells / 2;
	coarseCoord.second = pixelCoord.second / robotSizeInCells / 2;
	return coarseCoord;
}

realPosition Map::pixelToRobotPosition(Coordinate pixelCoord){
	realPosition robotPosition;
	robotPosition.first = (pixelCoord.first - 0.5 * mapHeight) * mapResolution * -1;
	robotPosition.second = (pixelCoord.second - 0.5 * mapWidth) * mapResolution;
	return robotPosition;
}

Coordinate Map::fineToPixelCoordinate(Coordinate fineGridCoord) {
	Coordinate pixelCoord;
	pixelCoord.first = fineGridCoord.first * robotSizeInCells + robotSizeInCells / 2;
	pixelCoord.second = fineGridCoord.second * robotSizeInCells + robotSizeInCells / 2;
	return pixelCoord;
}

void Map::inflateCell(int i, int j) {
	for (int x = -inflationRadius+i; x<= inflationRadius+i; x++) {
		for (int y = -inflationRadius+j; y<= inflationRadius+j; y++) {
			// bounds check
			if (x+i >= 0 && x < mapHeight && y >= 0 && y < mapWidth) {
				map[x][y] = true;
			}
		}
	}
}

void Map::loadMapFromFile(const char* filePath) {

	lodepng::decode(image, mapWidth, mapHeight, filePath);

	cout << "map size: " << mapWidth << "," << mapHeight << endl;

	map.resize(mapHeight);
	for (int i = 0; i < mapHeight; i++)
		map[i].resize(mapWidth);

	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			map[i][j] = checkIfCellIsOccupied(i, j);
		}
	}

}



bool Map::checkIfCellIsOccupied(int i, int j) {
	int c = (i * mapWidth + j) * 4;
	int r = image[c];
	int g = image[c + 1];
	int b = image[c + 2];

	if (r == 0 && g == 0 && b == 0)
		return true;
	return false;
}

void Map::reduceGrid(const Grid &originalGrid, Grid &reducedGrid, const int reduceConstant) {
	int originalGridHeight = originalGrid.size();
	int originalGridWidth = originalGrid[0].size();

	int gridRows = originalGridHeight / reduceConstant;
	int gridCols = originalGridWidth / reduceConstant;

	reducedGrid.resize(gridRows);
	for (int i = 0; i < gridRows; i++)
		reducedGrid[i].resize(gridCols);

	for (int i = 0; i < gridRows; i++) {
		for (int j = 0; j < gridCols; j++) {
			reducedGrid[i][j] = checkIfReducedCellIsOccupied(i, j, originalGrid, reduceConstant);
		}
	}

	printGrid(reducedGrid);
}

bool Map::checkIfReducedCellIsOccupied(int i, int j, const Grid &originalGrid, int reduceConstant) const {
	int startRow = i * reduceConstant;
	int startCol = j * reduceConstant;

	int gridHeight = originalGrid.size();
	int gridWidth = originalGrid[0].size();

	for (int x = startRow; x < startRow + reduceConstant; x++) {
		for (int y = startCol; y < startCol + reduceConstant; y++) {
			if (x >= gridHeight || y >= gridWidth) continue; // boundary check
			if (originalGrid[x][y]) return true;
		}
	}
	return false;
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

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << (grid[i][j] ? "*" : " ");
		}
		cout << endl;
	}
}

Map::~Map() {
	// TODO Auto-generated destructor stub
}

