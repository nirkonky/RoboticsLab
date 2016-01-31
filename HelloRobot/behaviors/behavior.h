
#ifndef BEHAVIOR_H_
#define BEHAVIOR_H_
#include "../defines.h"
#include "../robot.h"
#include <iostream>

using namespace std;

class behavior {

public:
	vector<behavior *> getNextBehavior;
	robot * robotObject;
	double factor;
	virtual string name() = 0;

	behavior *pushNext(behavior *next);
	behavior *getNext(realPosition targetWaypoint, double angle);
	virtual bool start(realPosition targetWaypoint, double angle) = 0;
	virtual bool stop(realPosition targetWaypoint, double angle) = 0;
	virtual void moving() = 0;

	behavior(robot *robotb);


	static void printRealPos(realPosition a)
	{
		cout << "{ x:" << a.second << ", y:" << a.first << "}" << endl;
	}

	static int canRight(double current, double target)
	{
		double difference = current - target;
		while (difference < -180)
			difference += 360;
		while (difference > 180)
			difference -= 360;
		if (difference > 0)
		{
			cout << "in "<< endl;
			return true;
		}
		cout << "out "<< endl;
		return false;
	}
	virtual ~behavior();
};

#endif /* BEHAVIOR_H_ */
