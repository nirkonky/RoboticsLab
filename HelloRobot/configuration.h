
#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_


#include <string>
#include "robotState.h"

class robotState;

using namespace std;

class configuration {
private:
	string mapPath;
	robotState initialRobotPosition;
	float robotSizeCM;
	float mapResolutionCM;
public:
	configuration(const char* filePath);
	string getMapPath();
	robotState getInitialRobotPosition();
	float getRobotSizeCM();
	float getMapResolutionCM();
	virtual ~configuration();
};

#endif
