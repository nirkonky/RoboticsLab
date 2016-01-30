/*
 * behavior.h
 *
 *  Created on: Jan 30, 2016
 *      Author: colman
 */

#ifndef BEHAVIOR_H_
#define BEHAVIOR_H_
#include "../defines.h"
#include "../robot.h"
#include <iostream>

using namespace std;

class behavior {
private:
	vector<behavior *> _nextBehaviors;

protected:
	robot *_robot;

	//int Counter;
	//Behavior** nextBehavior;

public:
	double _multiplier;
	virtual bool startCond(realPosition targetWaypoint, double angle) = 0;
	virtual bool stopCond(realPosition targetWaypoint, double angle) = 0;

	virtual void action() = 0;

	behavior *addNext(behavior *next);
	behavior *selectNext(realPosition targetWaypoint, double angle);
	virtual string name() = 0;
	behavior(robot *robot);
	virtual ~behavior();
	static double radiansToDegrees(double radians){
			return radians * 180 / M_PI;
		}

		static double degressToRadians(double degrees){
			return degrees * M_PI / 180;
		}

		static int isCloseEnough(realPosition a, realPosition b){
			double x_a = a.second;
			double y_a = a.first;
			double x_b = b.second;
			double y_b = b.first;

			if ((abs(x_a - x_b) <  LINEAR_TOLERANCE) &&
					(abs(y_a - y_b) < LINEAR_TOLERANCE)){
				return 0; // VERY CLOSE
			}
			else if ((abs(x_a - x_b) <  10 * LINEAR_TOLERANCE) &&
					(abs(y_a - y_b) < 10 * LINEAR_TOLERANCE)){
				return 1; // CLOSE! CAREFUL!
			}
			return 2; // FAR AWAY.
		}

		static void printRealPosition(realPosition a){
			cout << "{ x:" << a.second << ", y:" << a.first << "}" << endl;
		}

		static int isAngleCloseEnough(realPosition a, double yaw, realPosition b){
			double angle = radiansToDegrees(atan2( (b.first - a.first),b.second - a.second));
			if (abs(yaw - angle) < ANGLE_TOLERANCE){
				return 0; // VERY CLOSE;
			}
			else if (abs(yaw - angle) < ANGLE_TOLERANCE * 40){
				return 1; // CLOSE! CAREFUL!
			}
			return 2; // FAR AWAY;
		}

		static int isRight(double current, double target){
				double difference = current - target;

				while (difference < -180)
					difference += 360;

				while (difference > 180)
					difference -= 360;

				//cout  << "Target: " << target << " Current: " << current << " Difference: " << difference << " -> ";

				if (difference > 0){
					 cout << "in "<< endl;
					return true;
				}
				 cout << "out "<< endl;
				return false;
			}

};

#endif /* BEHAVIOR_H_ */
