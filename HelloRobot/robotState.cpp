
#include "robotState.h"

robotState::robotState(){}

robotState::robotState(double x, double y, double yaw): x(x), y(y), yaw(yaw) {}

void robotState::setX(double setX){this->x=setX;}
void robotState::setY(double setY){this->y=setY;}
void robotState::setYaw(double setYaw){this->yaw = setYaw;}

double robotState::getX(){return x;}
double robotState::getY(){return y;}
double robotState::getYaw(){return yaw;}

robotState::~robotState(){}
