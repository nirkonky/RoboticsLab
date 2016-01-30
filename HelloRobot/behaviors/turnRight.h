/*
 * turnRight.h
 *
 *  Created on: Jan 30, 2016
 *      Author: colman
 */

#ifndef TURNRIGHT_H_
#define TURNRIGHT_H_

#include "../robot.h"
#include "behavior.h"

using namespace std;

class turnRight: public behavior {
public:
	turnRight(robot* robot):behavior(robot){};
	bool startCond(realPosition targetWaypoint, double angle);
	bool stopCond(realPosition targetWaypoint, double angle);
	void action(){_robot->setSpeed(0.0,-1 * TURN_SPEED * _multiplier);}
	void pause();
	string name(){cout << "Turn Right:" << endl; return "TurnRight";}
};
#endif /* TURNRIGHT_H_ */
