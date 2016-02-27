
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

	void printGrid(const Grid &grid) const;
	void reduceGrid(const Grid &originalGrid, Grid &reducedGrid, int reduceConstant);
	void inflateCell(int i, int j);

	Grid fineGrid;
	Grid coarseGrid;



public:
	Map(float mapResolution, float robotSize);
	unsigned int mapWidth;
	unsigned int mapHeight;
	float mapResolution;
	robotBehavior coarseToCordy(robotBehavior coarseToCord);
	robotBehavior pixelToCordy(robotBehavior pixelToCord);
	robotBehavior fineToCordy(robotBehavior fineToCord);
	void loadMapFromFile(const char* filePath);
	void saveMapToFile(const char* filePath);
	void inflateMap();
	void buildFineGrid();
	void buildCoarseGrid();
	realPosition pixelToRobotPosition(robotBehavior pixelToRobotPos);
	Grid getMapGrid();
	Grid getCoarseGrid();
	int getCoarseGridPixelWidth();
	int getFineGridPixelWidth();
	virtual ~Map();
};

#endif /* MAP_H_ */
