/*
 * robotState.cpp
 *
 *  Created on: Jan 30, 2016
 *      Author: colman
 */

#include "robotState.h"

robotState::robotState(double x, double y, double yaw): x(x), y(y), yaw(yaw) {

}

double robotState::getX() {
	return x;
}
double robotState::getY() {
	return y;
}
double robotState::getYaw() {
	return yaw;
}

robotState::~robotState() {
	// TODO Auto-generated destructor stub
}
