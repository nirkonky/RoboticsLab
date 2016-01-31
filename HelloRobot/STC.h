#ifndef STC_H_
#define STC_H_

#include "Map.h"
#include "Node.h"
#include "defines.h"
using namespace std;


class STC {
private:
	Map &map;
	vector<vector<Node *> > workingMap;
	Position startPosition;

	void DFS(Node* n);
	void drawLineOnImage(vector<unsigned char> &image, Position positionA, Position positionB, int mapWidth);
	void drawSpanningTree(vector<unsigned char> &image, int mapWidth);
	Position fineGridCoordinate(Position coarseGridCoord, int corner);
	void fillPath(vector<Position> &path, Node *coarseGridNode, Position fineGridCoord, int corner, Position initialFineGridPosition);

public:
	STC(Map &map, Position startPos);
	void printWorkingMap();
	void printDFSFunc();
	void buildWorkingMap();
	vector<Position> walkingPath();
	vector<realPosition> getRealWalkingPath();
	void saveDFSToFile(const char* filePath);
	virtual ~STC();
};

#endif /* STC_H_ */
