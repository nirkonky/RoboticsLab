#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include "configuration.h"
#include "defines.h"

using namespace std;

configuration::configuration(const char* filePath): initialRobotPosition(0, 0, 0) {
	ifstream confFile(filePath);
	string line;
	string parameterName;

	// map path
	string mapPath;
	getline(confFile, line);
	istringstream mapPathStream(line);
	mapPathStream >> parameterName >> mapPath;
	this->mapPath = mapPath;

	// initial robot position
	string initXStr, initYStr, initYawStr;
	getline(confFile, line);
	istringstream initPositionStream(line);
	initPositionStream >> parameterName >> initYStr >> initXStr >> initYawStr;
	int initX = atoi(initXStr.c_str());
	int initY = atoi(initYStr.c_str());
	int initYaw = atoi(initYawStr.c_str());
	initialRobotPosition = robotState(initX, initY, initYaw);

	// robot size (assume to be squared)
	string robotSizeCMStr;
	getline(confFile, line);
	istringstream robotSizeStream(line);
	robotSizeStream >> parameterName >> robotSizeCMStr;
	robotSizeCM = atof(robotSizeCMStr.c_str());

	// map resolution
	string mapResolutionCMStr;
	getline(confFile, line);
	istringstream mapResolutionStream(line);
	mapResolutionStream >> parameterName >> mapResolutionCMStr;
	mapResolutionCM = atof(mapResolutionCMStr.c_str());
}

string configuration::getMapPath() {
	return mapPath;
}

robotState configuration::getInitialRobotPosition() {
	return initialRobotPosition;
}

float configuration::getRobotSizeCM() {
	return robotSizeCM;
}

float configuration::getMapResolutionCM() {
	return mapResolutionCM;
}

configuration::~configuration() {
	// TODO Auto-generated destructor stub
}
