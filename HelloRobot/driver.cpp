
#include "defines.h"
#include "driver.h"
#include "behaviorForward.h"
#include "behaviorLeft.h"
#include "behaviorRight.h"

#include <math.h>

driver::driver(robot *robot, vector<realPosition> waypoints) : robotObject(robot), wayPointsList(waypoints) {
	currBehavior = NULL;
	//0 -> forward -> 1,2
	//1 -> left -> 0
	//2 ->right ->0
	behaviorList[2] = new behaviorLeft(robot);
	behaviorList[0] = new behaviorForward(robot);
	behaviorList[1] = new behaviorRight(robot);
	behaviorList[0]->pushNext(behaviorList[1]);
	behaviorList[0]->pushNext(behaviorList[2]);
	behaviorList[2]->pushNext(behaviorList[0]);
	behaviorList[1]->pushNext(behaviorList[0]);
}

void driver::run()
{
	//start reading.
	robotObject->read();
	int wayPointCounter = 0;
	realPosition currentWaypoint;
	realPosition nextWaypoint;
	nextWaypoint = this->wayPointsList[wayPointCounter];
	double angle = (atan2((nextWaypoint.first - robotObject->getYPos()),nextWaypoint.second - robotObject->getXPos())*180 / M_PI);
	currBehavior = behaviorList[1];//start with turning left.

	while (wayPointCounter < wayPointsList.size())
	{
		//keep reading.
		while (!currBehavior->stop(nextWaypoint, angle))
		{
			currBehavior->moving();
		}

		if (currBehavior->name() == "behavior Forward")
		{
			cout<<"in the move forward!"<<endl;
			wayPointCounter++;
			nextWaypoint = this->wayPointsList[wayPointCounter];
			angle = (atan2((nextWaypoint.first - robotObject->getYPos()),nextWaypoint.second - robotObject->getXPos())*180 / M_PI);
		}
		cout << "Next wayPoint: ";
		cout << "{ x:" << nextWaypoint.second << ", y:" << nextWaypoint.first << "}" << endl;
		cout << "Waypoint number: "<< wayPointCounter << endl;
		currBehavior = currBehavior->getNext(nextWaypoint, angle);
		cout<<"angle:" <<angle<<endl;
}
cout << "Manager stopped." << endl;
}
driver::~driver() {for (int i = 0; i < BEHAVIORS_ARRAY_SIZE; i++) delete behaviorList[i];}

