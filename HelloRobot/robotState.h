
#ifndef ROBOTSTATE_H_
#define ROBOTSTATE_H_


#include "defines.h"
using namespace std;

class robotState {
private:
	double x, y, yaw;
public:
	robotState();
	robotState(double x, double y, double yaw);
	double getX();
	double getY();
	double getYaw();
	void setX(double x);
	void setY(double y);
	void setYaw(double Yaw);
	virtual ~robotState();
};

#endif /* ROBOTPOSITION_H_ */
