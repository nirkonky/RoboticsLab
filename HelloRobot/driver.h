
#ifndef DRIVER_H_
#define DRIVER_H_
#include "defines.h"
#include "robot.h"
#include "behaviors/behavior.h"
#include <vector>

using namespace std;



class driver {
private:
	robot *_robot;
	behavior *_currBehavior;
	behavior *_behaviorsArray[BEHAVIORS_ARRAY_SIZE];
	vector<realPosition> _waypoints;
	float _laserScanArray[SAMPLES];

public:

	driver(robot *robot, vector<realPosition> wayPoints);
	void run();
	virtual ~driver();
};
#endif /* DRIVER_H_ */
