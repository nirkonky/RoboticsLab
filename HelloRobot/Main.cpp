
#include "Map.h"
#include "STC.h"
#include "wayPointManager.h"
#include "configuration.h"
#include "robot.h"
#include "robotState.h"
#include "driver.h"
#include <iostream>

using namespace std;

void printCoordinate(coordinatePlace coord) {
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

		coordinatePlace pixelCoord(initRobotPosition.getX(), initRobotPosition.getY());

		printCoordinate(pixelCoord);
		// translate to coarse grid
		realPosition robotState = map.pixelToRobotPosition(pixelCoord);
		Position startPos = map.pixelToCoarseCoordinate(pixelCoord);
		printCoordinate(startPos);
		printCoordinate(map.coarseToPixelCoordinate(startPos));
		STC stc(map, startPos);
		stc.buildWorkingMap();
		stc.printWorkingMap();
		stc.saveDFSToFile("roboticLabMap_spanningTree.png");
		vector<realPosition> path = stc.getRealWalkingPath();
		wayPointManager wpm(path);
		vector<realPosition> waypoints = wpm.getWaypoints();
		wpm.printWaypoints();
		robot robot("localhost", 6665, robotState.second, robotState.first, initRobotPosition.getYaw()* M_PI / 180);
		driver driver(&robot, waypoints);
		driver.run();
		return 0;
}


