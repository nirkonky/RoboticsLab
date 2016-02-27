
#include "Map.h"
#include "STC.h"
#include "wayPointManager.h"
#include "robot.h"
#include "behavior.h"

#include "defines.h"
#include "behaviorForward.h"
#include "behaviorLeft.h"
#include "behaviorRight.h"
#include "robotState.h"
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

void printRobotBehavior(robotBehavior coordinate)
{
	cout << "(" << coordinate.first << "," << coordinate.second << ")" << endl;
}

void getConfiguration(const char* filePath){
	ifstream confFile(filePath);
	string line;
	string parameterName;
	// map path
	getline(confFile, line);
	istringstream mapPathStream(line);
	mapPathStream >> parameterName >> mapPathFromFile;

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

	string robotSizeStr;
	getline(confFile, line);
	istringstream robotSizeStream(line);
	robotSizeStream >> parameterName >> robotSizeStr;
	robotSizeFromFile = atof(robotSizeStr.c_str());

	string mapResolutionStr;
	getline(confFile, line);
	istringstream mapResolutionStream(line);
	mapResolutionStream >> parameterName >> mapResolutionStr;
	mapSizeFromFile = atof(mapResolutionStr.c_str());
}
int main() {

	//reading the params from thepara.txt robot params.
	getConfiguration("para.txt");
	float mapResolution = mapSizeFromFile / 100;
	float robotSize = robotSizeFromFile / 100;
	//initalize the map with the params.
	Map map(mapResolution, robotSize);
	map.loadMapFromFile(mapPathFromFile.c_str());
	map.buildFineGrid();
	map.buildCoarseGrid();

	//Initial the robotState(x,y).
	robotBehavior robotInitialState(robotInitialFromFile.getX(), robotInitialFromFile.getY());

	printRobotBehavior(robotInitialState);
	//from pixel to robotPosition.
	realPosition robotState = map.pixelToRobotPosition(robotInitialState);
	//from pixel to Coordinate.
	position startPos = map.pixelToCordy(robotInitialState);


	printRobotBehavior(startPos);

	printRobotBehavior(map.coarseToCordy(startPos));
	STC stc(map, startPos);
	stc.buildWorkingMap();
	stc.printWorkingMap();

	stc.saveDFSToFile("roboticLabMap_spanningTree.png");
	//get the path that the robot need to move.
	vector<realPosition> path = stc.getRealWalkingPath();

	wayPointManager wpm(path);

	vector<realPosition> waypoints = wpm.getWaypoints();
	wpm.printWaypoints();
	robot robot("localhost", 6665, robotState.second, robotState.first, robotInitialFromFile.getYaw()* M_PI / 180);
	//start the driver.
	//robot.
	behavior *currBehavior;
	behavior *behaviorList[3];

	behaviorList[2] = new behaviorLeft(&robot);
	behaviorList[0] = new behaviorForward(&robot);
	behaviorList[1] = new behaviorRight(&robot);

	behaviorList[0]->pushNext(behaviorList[1]);
	behaviorList[0]->pushNext(behaviorList[2]);
	behaviorList[2]->pushNext(behaviorList[0]);
	behaviorList[1]->pushNext(behaviorList[0]);

	//start reading.
		robot.read();
		int wayPointCounter = 0;
		realPosition currentWaypoint;
		realPosition nextWaypoint;
		nextWaypoint = waypoints[wayPointCounter];
		double angle = (atan2((nextWaypoint.first - robot.getYPos()),nextWaypoint.second - robot.getXPos())*180 / M_PI);
		currBehavior = behaviorList[1];//start with turning left.

		while (wayPointCounter < waypoints.size())
		{
			cout<<"im here!!!"<<endl;

			//keep reading.
			while (!currBehavior->stop(nextWaypoint, angle))
			{
				currBehavior->moving();
			}

			if (currBehavior->name() == "behavior Forward")
			{
				cout<<"in the move forward!"<<endl;
				wayPointCounter++;
				nextWaypoint = waypoints[wayPointCounter];
				angle = (atan2((nextWaypoint.first - robot.getYPos()),nextWaypoint.second - robot.getXPos())*180 / M_PI);
			}
			cout << "Next wayPoint: ";
			cout << "[" << nextWaypoint.second << "," << nextWaypoint.first << "]" << endl;
			cout<<"With Angle:" <<angle<<endl;
			cout << "Waypoint number: "<< wayPointCounter << endl;
			currBehavior = currBehavior->getNext(nextWaypoint, angle);
		  }
	cout << "Manager stopped." << endl;
	return 0;
	}


