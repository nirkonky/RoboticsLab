

#ifndef WAYPOINTMANAGER_H_
#define WAYPOINTMANAGER_H_
#include <vector>
#include "defines.h"
using namespace std;

class wayPointManager {
public:
	wayPointManager(vector<realPosition> &path);
	void printWaypoints();
	vector<realPosition> pathToWayPoints;
	vector<realPosition> wayPoints;
	vector<realPosition> getWaypoints();
	bool delta(double a,double b);
	virtual ~wayPointManager();
};

#endif /* WAYPOINTMANAGER_H_ */
