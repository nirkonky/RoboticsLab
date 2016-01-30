/*
 * Main.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: colman
 */

#include "Map.h"
#include "STC.h"
#include "wayPointManager.h"
#include "configuration.h"
#include "robot.h"
#include "robotState.h"
#include "driver.h"
#include <iostream>

using namespace std;

void printCoordinate(Coordinate coord) {
	cout << "(" << coord.first << "," << coord.second << ")" << endl;
}
int main() {
	configuration config("para.txt");
	robotState initRobotPosition = config.getInitialRobotPosition();

		cout << "map path: " << config.getMapPath() << endl;
		cout << "initial robot position: (" << initRobotPosition.getX() << "," << initRobotPosition.getY() << "," << initRobotPosition.getYaw() << ")" << endl;
		cout << "robot size: " << config.getRobotSizeCM() << endl;
		cout << "map resolution: " << config.getMapResolutionCM() << endl;

		float mapResolution = config.getMapResolutionCM() / 100;
		float robotSize = config.getRobotSizeCM() / 100;
		Map map(mapResolution, robotSize);

		map.loadMapFromFile(config.getMapPath().c_str());

		map.inflateMap();

		map.buildFineGrid();
		map.buildCoarseGrid();

		// configure start position

		Coordinate pixelCoord(initRobotPosition.getX(), initRobotPosition.getY());

		printCoordinate(pixelCoord);
		// translate to coarse grid
		realPosition robotState = map.pixelToRobotPosition(pixelCoord);
		Position startPos = map.pixelToCoarseCoordinate(pixelCoord);
		printCoordinate(startPos);
		printCoordinate(map.coarseToPixelCoordinate(startPos));
		STC stc(map, startPos);
		stc.buildGraph();
		stc.printGraph();
		stc.saveSpanningTreeToFile("roboticLabMap_spanningTree.png");
		vector<realPosition> path = stc.realPath();
		cout<<"numbers of real positions is: "<< path.size()<<endl;
		wayPointManager wpm(path);
		vector<realPosition> waypoints = wpm.getWaypoints();

		wpm.printWaypoints();

		//stc.savePathToFile(waypoints, "roboticLabMap_path.png");
		robot robot("localhost", 6665, robotState.second, robotState.first, behavior::degressToRadians(initRobotPosition.getYaw()));
		driver driver(&robot, waypoints);
		driver.run();
		cout<<"printWAy Pointsss"<< waypoints.size()<<endl;

		return 0;
}


