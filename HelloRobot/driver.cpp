/*
 * driver.cpp
 *
 *  Created on: Jan 30, 2016
 *      Author: colman
 */

#include "defines.h"
#include "driver.h"
#include "behaviors/moveForward.h"
#include "behaviors/turnLeft.h"
#include "behaviors/turnRight.h"

#include <math.h>

driver::driver(robot *robot, vector<realPosition> waypoints) : _robot(robot), _waypoints(waypoints) {
	_currBehavior = NULL;
	_behaviorsArray[0] = new turnLeft(robot);
	_behaviorsArray[1] = new moveForward(robot);
	_behaviorsArray[2] = new turnRight(robot);
	_behaviorsArray[0]->addNext(_behaviorsArray[1]);
	_behaviorsArray[2]->addNext(_behaviorsArray[1]);
	_behaviorsArray[1]->addNext(_behaviorsArray[0]);
	_behaviorsArray[1]->addNext(_behaviorsArray[2]);

}

void driver::run()
{

	double yaw;
	_robot->read();

	int nextWaypointCounter = 0;
	realPosition currentWaypoint;
	realPosition nextWaypoint;

	currentWaypoint.first = _robot->getYPos();
	currentWaypoint.second = _robot->getXPos();
	yaw = _robot->getYaw();
	nextWaypoint = this->_waypoints[nextWaypointCounter];
	//cout << "var p1 = { x:" << currentWaypoint.second << ", y:" << currentWaypoint.first << "}" << endl;
	//cout << "var p2 = { x:" << nextWaypoint.second << ", y:" << nextWaypoint.first << "}" << endl;
	double angle = behavior::radiansToDegrees(atan2( (nextWaypoint.first - currentWaypoint.first),nextWaypoint.second - currentWaypoint.second));

	/*for (int i = 0; i < BEHAVIORS_ARRAY_SIZE; i++) {
		if (_behaviorsArray[i] != NULL){
			if (_behaviorsArray[i]->startCond(nextWaypoint, angle)){
				_currBehavior = _behaviorsArray[i];
				break;
			}
		}
	}*/

	_currBehavior = _behaviorsArray[2];

	if (!(_currBehavior->startCond(nextWaypoint, angle))){
		return;
	}

	while (_currBehavior && nextWaypointCounter < _waypoints.size()) {
		_robot->read();
		while (!_currBehavior->stopCond(nextWaypoint, angle)) {
			_currBehavior->action();
			_robot->read();
		}

		currentWaypoint.first = _robot->getYPos();
		currentWaypoint.second = _robot->getXPos();
		cout << "Waypoint number: "<< nextWaypointCounter << endl;
		string stam = _currBehavior->name();
		if (_currBehavior->name() == "Move Forward") {
			nextWaypointCounter++;
			//currentWaypoint = this->_waypoints[nextWaypointCounter];
			nextWaypoint = this->_waypoints[nextWaypointCounter];
			angle = behavior::radiansToDegrees(atan2((nextWaypoint.first - currentWaypoint.first),nextWaypoint.second - currentWaypoint.second));
		}
		cout << "Next: ";
		behavior:: printRealPosition(nextWaypoint);
		_currBehavior = _currBehavior->selectNext(nextWaypoint, angle);
}
cout << "Manager stopped." << endl;
}
driver::~driver() {for (int i = 0; i < BEHAVIORS_ARRAY_SIZE; i++) delete _behaviorsArray[i];}

