#include "wayPointManager.h"
#include <iostream>

using namespace std;

wayPointManager::wayPointManager(vector<realPosition> &path) : pathToWayPoints(path)
{
	this->wayPoints = path;
}
bool wayPointManager::delta(double a, double b){
	if((a-b)==0){
		return true;
	}
	else
	{
		return false;
	}
}

vector<realPosition> wayPointManager::getWaypoints()
{
	int wayPointCounter = 0;
	bool isXChanged;
	double delta_x, delta_y;
	vector<realPosition> waypoints;
	waypoints.resize(pathToWayPoints.size());
	if (!pathToWayPoints.empty())
	{
		waypoints[0] = pathToWayPoints[0];
		if (delta(pathToWayPoints[0].first,pathToWayPoints[1].first))
		{
			isXChanged = false;
		}
		else
		{
			isXChanged = true;
		}
		wayPointCounter ++;
	}
	for (int i = 1; i < pathToWayPoints.size()-1; ++i)
	{
		if (delta(pathToWayPoints[i].first,pathToWayPoints[i+1].first))
		{
			if (isXChanged)
			{
				waypoints[wayPointCounter] = pathToWayPoints[i];
				wayPointCounter ++;
			}
			isXChanged = false;
		}
		else if (delta(pathToWayPoints[i].second,pathToWayPoints[i+1].second))
		{
			if (!isXChanged)
			{
				waypoints[wayPointCounter] = pathToWayPoints[i];
				wayPointCounter ++;
			}

			isXChanged = true;
		}
	}
	waypoints.resize(wayPointCounter);
	this->wayPoints = waypoints;
	return waypoints;
}


void wayPointManager::printWaypoints(){
	for (int i = 0; i < this->wayPoints.size()-1; ++i)
	{
		cout << "puck( pose [" << wayPoints[i].second << " " << wayPoints[i].first << " 0 ] color \"red\" )" << endl;
	}
}

wayPointManager::~wayPointManager() {}

