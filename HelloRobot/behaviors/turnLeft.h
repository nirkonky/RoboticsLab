/*
 * turnLeft.h
 *
 *  Created on: Jan 30, 2016
 *      Author: colman
 */

#ifndef TURNLEFT_H_
#define TURNLEFT_H_

#include "../robot.h"
#include "behavior.h"

using namespace std;

class turnLeft: public behavior {
public:
	turnLeft(robot* robot): behavior(robot){};
	bool startCond(realPosition targetWaypoint, double angle);
	bool stopCond(realPosition targetWaypoint, double angle);
	void action(){_robot->setSpeed(0.0,TURN_SPEED * _multiplier);}
	void pause();
	string name(){cout << "Turn Left:" << endl; return "TurnLeft";}
};
#endif /* TURNLEFT_H_ */
