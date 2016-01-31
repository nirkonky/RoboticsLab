

#ifndef WAYPOINTMANAGER_H_
#define WAYPOINTMANAGER_H_
#include <vector>
#include "defines.h"
using namespace std;

class wayPointManager {
public:
	vector<realPosition> pathToWayPoints;
	vector<realPosition> wayPoints;
	wayPointManager(vector<realPosition> &path);

	vector<realPosition> getWaypoints();

	void printWaypoints();

	virtual ~wayPointManager();
};

#endif /* WAYPOINTMANAGER_H_ */
