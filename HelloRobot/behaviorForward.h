/*
 * behaviorForward.h
 *
 *  Created on: Feb 3, 2016
 *      Author: colman
 */

#ifndef BEHAVIORFORWARD_H_
#define BEHAVIORFORWARD_H_



#include "robot.h"
#include "behavior.h"
#include "defines.h"


using namespace std;


class behaviorForward: public behavior {
//class behaviorForward: public behavior {

public:

	realPosition currentWaypoint;

	behaviorForward(robot* robotb): behavior(robotb){};

	string name()
	{
		return "behavior Forward";
	}


	bool start(realPosition targetWaypoint, double degree);

	bool stop(realPosition targetWaypoint, double degree);

	void moving()
	{
		robotObject->setSpeed(LINEAR_SPEED * factor,0.0);
		robotObject->read();
	}

	virtual ~behaviorForward();
};


#endif /* BEHAVIORFORWARD_H_ */
