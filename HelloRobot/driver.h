
#ifndef DRIVER_H_
#define DRIVER_H_
#include "defines.h"
#include "robot.h"
#include "behavior.h"
#include <vector>

using namespace std;



class driver {
private:
	robot *robotObject;
	behavior *currBehavior;
	behavior *behaviorList[BEHAVIORS_ARRAY_SIZE];
	vector<realPosition> wayPointsList;
	float laserScanAreaList[SAMPLES];

public:

	driver(robot *robot, vector<realPosition> wayPoints);
	void run();
	virtual ~driver();
};
#endif /* DRIVER_H_ */
