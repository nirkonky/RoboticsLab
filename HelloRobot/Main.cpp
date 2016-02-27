
#include "Map.h"
#include "STC.h"
#include "wayPointManager.h"
#include "robot.h"
#include "robotState.h"
#include "driver.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
using namespace std;

robotState robotInitialFromFile;
string mapPathFromFile;
float mapSizeFromFile;
float robotSizeFromFile;

void printRobotBehavior(robotBehavior cordy)
{
	cout << "(" << cordy.first << "," << cordy.second << ")" << endl;
}

void getConfiguration(const char* filePath){
	ifstream confFile(filePath);
	string line;
	string parameterName;
	cout<<"imhere!"<<endl;
	// map path
	getline(confFile, line);
	istringstream mapPathStream(line);
	mapPathStream >> parameterName >> mapPathFromFile;
	//this->mapPath = mapPath;

	// initial robot position
	string initXStr, initYStr, initYawStr;
	getline(confFile, line);
	istringstream initPositionStream(line);
	initPositionStream >> parameterName >> initYStr >> initXStr >> initYawStr;
	int initX = atoi(initXStr.c_str());
	int initY = atoi(initYStr.c_str());
	int initYaw = atoi(initYawStr.c_str());
	robotInitialFromFile.setX(initX);
	robotInitialFromFile.setY(initY);
	robotInitialFromFile.setYaw(initYaw);

	// robot size (assume to be squared)
	string robotSizeStr;
	getline(confFile, line);
	istringstream robotSizeStream(line);
	robotSizeStream >> parameterName >> robotSizeStr;
	robotSizeFromFile = atof(robotSizeStr.c_str());

	// map resolution
	string mapResolutionStr;
	getline(confFile, line);
	istringstream mapResolutionStream(line);
	mapResolutionStream >> parameterName >> mapResolutionStr;
	mapSizeFromFile = atof(mapResolutionStr.c_str());
}
int main() {

	getConfiguration("para.txt");
	float mapResolution = mapSizeFromFile / 100;
	float robotSize = robotSizeFromFile / 100;
	Map map(mapResolution, robotSize);

	map.loadMapFromFile(mapPathFromFile.c_str());

	map.inflateMap();

	map.buildFineGrid();
	map.buildCoarseGrid();

	// configure start position

	robotBehavior pixelCoord(robotInitialFromFile.getX(), robotInitialFromFile.getY());

	printRobotBehavior(pixelCoord);
	// translate to coarse grid
	realPosition robotState = map.pixelToRobotPosition(pixelCoord);
	position startPos = map.pixelToCordy(pixelCoord);
	printRobotBehavior(startPos);
	printRobotBehavior(map.coarseToCordy(startPos));
	STC stc(map, startPos);
	stc.buildWorkingMap();
	stc.printWorkingMap();
	stc.saveDFSToFile("roboticLabMap_spanningTree.png");
	vector<realPosition> path = stc.getRealWalkingPath();
	wayPointManager wpm(path);
	vector<realPosition> waypoints = wpm.getWaypoints();
	wpm.printWaypoints();
	robot robot("localhost", 6665, robotState.second, robotState.first, robotInitialFromFile.getYaw()* M_PI / 180);
	driver driver(&robot, waypoints);
	driver.run();
	return 0;
}


