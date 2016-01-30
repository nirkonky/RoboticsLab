/*
 * wayPointManager.h
 *
 *  Created on: Jan 30, 2016
 *      Author: colman
 */

#ifndef WAYPOINTMANAGER_H_
#define WAYPOINTMANAGER_H_
#include <vector>
#include "defines.h"
using namespace std;

class wayPointManager {
public:
	vector<realPosition> path;
	vector<realPosition> waypoints;
	wayPointManager(vector<realPosition> &path);

	vector<realPosition> getWaypoints();

	void printWaypoints();

	virtual ~wayPointManager();
};

#endif /* WAYPOINTMANAGER_H_ */
