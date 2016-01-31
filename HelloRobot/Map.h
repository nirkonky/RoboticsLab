
#ifndef MAP_H_
#define MAP_H_
#include "defines.h"
#include <vector>
using namespace std;

typedef vector<vector<bool> > Grid;

class Map {
private:
	vector<unsigned char> image;
	Grid map;

	float robotSize;
	int robotSizeInCells;
	int inflationRadius;

	Grid fineGrid; // each cell in the size of the robot
	Grid coarseGrid; // each cell in the size of 2x2 of the robot

	void printGrid(const Grid &grid) const;
	void reduceGrid(const Grid &originalGrid, Grid &reducedGrid, int reduceConstant);
	void inflateCell(int i, int j);

public:
	unsigned int mapWidth;
	unsigned int mapHeight;
	float mapResolution;
	coordinatePlace coarseToPixelCoordinate(coordinatePlace coarseGridCoord);
	realPosition pixelToRobotPosition(coordinatePlace pixelCoord);
	coordinatePlace pixelToCoarseCoordinate(coordinatePlace pixelCoord);
	coordinatePlace fineToPixelCoordinate(coordinatePlace fineGridCoord);
	Map(float mapResolution, float robotSize);
	void loadMapFromFile(const char* filePath);
	void saveMapToFile(const char* filePath);
	void inflateMap();
	Grid getMapGrid();
	int getCoarseGridPixelWidth();
	int getFineGridPixelWidth();
	void buildFineGrid();
	void buildCoarseGrid();
	Grid getCoarseGrid();
	virtual ~Map();
};

#endif /* MAP_H_ */
