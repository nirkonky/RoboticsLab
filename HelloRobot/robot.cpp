
#include "robot.h"
robot::robot(string ip, int port, double x, double y, double yaw) {
	pc = new PlayerClient(ip, port);
	pp = new Position2dProxy(pc);
	lp = new LaserProxy(pc);
	pp->SetOdometry(x,y,yaw);
	for (int i = 0; i<20; i++){
		read();
	}
}

void robot::setSpeed(double linearSpeed, double angularSpeed){
	pp -> SetSpeed(linearSpeed, angularSpeed);
}

double robot::getXPos(){
	return pp -> GetXPos();
}

double robot::getYPos(){
	return pp -> GetYPos();
}

double robot::getYaw(){
	return pp -> GetYaw();
}

void robot::read(){
	pc -> Read();
}

robot::~robot() {
	delete lp;
	delete pp;
	delete pc;
}
