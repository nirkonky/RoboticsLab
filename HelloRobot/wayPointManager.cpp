#include "wayPointManager.h"
#include <iostream>

using namespace std;

wayPointManager::wayPointManager(vector<realPosition> &path) : pathToWayPoints(path)
{
	this->wayPoints = path;
}

vector<realPosition> wayPointManager::getWaypoints()
{
	int counter = 0;
	bool isXChanged;
	double delta_x, delta_y;
	vector<realPosition> waypoints;
	waypoints.resize(pathToWayPoints.size());
	// handle first node is a waypoint exception
	if (pathToWayPoints.size() > 2)
	{
		delta_x = pathToWayPoints[0].first - pathToWayPoints[1].first;
		delta_y = pathToWayPoints[0].second - pathToWayPoints[1].second;
		waypoints[0] = pathToWayPoints[0];
		counter += 1;
		isXChanged = (delta_x == 0) ? false : true;
	}
	for (int i = 1; i < pathToWayPoints.size()-1; ++i)
	{
		// check movement
		delta_x = pathToWayPoints[i].first - pathToWayPoints[i+1].first;
		delta_y = pathToWayPoints[i].second - pathToWayPoints[i+1].second;
		//there is change in Y.
		if (delta_x == 0)
		{
			if (isXChanged)
			{
				waypoints[counter] = pathToWayPoints[i];
				counter += 1;
			}
			isXChanged = false;
		}
		//there is change in X.
		else if (delta_y == 0)
		{
			if (!isXChanged)
			{
				waypoints[counter] = pathToWayPoints[i];
				counter += 1;
			}

			isXChanged = true;
		}
	}

	waypoints.resize(counter);
	this->wayPoints = waypoints;
	return waypoints;
}


void wayPointManager::printWaypoints(){
	cout<<this->wayPoints.size()<<endl;
	for (unsigned int i = 0; i < this->wayPoints.size()-1; ++i)
	{
		cout << "puck( pose [" << wayPoints[i].second << " " << wayPoints[i].first << " 0 ] color \"red\" )" << endl;
	}
}

wayPointManager::~wayPointManager() {}

