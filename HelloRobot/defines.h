/*
 * defines.h
 *
 *  Created on: Jan 30, 2016
 *      Author: colman
 */

#include <vector>
#include <math.h>


#ifndef DEFINES_H_
#define DEFINES_H_
using namespace std;

	typedef vector<vector<bool> > Grid;
	typedef pair<int, int> Coordinate;
	typedef pair<int, int> Position;
	typedef pair<double, double> realPosition;
	const double LINEAR_SPEED = 0.3;
	const double TURN_SPEED = 0.4;
	const double LINEAR_TOLERANCE = 0.01;
	const double ANGLE_TOLERANCE = 0.4;

	#define SAMPLES 666
	#define BEHAVIORS_ARRAY_SIZE 3



#endif /* DEFINES_H_ */
