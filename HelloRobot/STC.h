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
	position startPosition;
	void DFS(Node* n);
	void fillPath(vector<position> &path, Node *correntNode, position positionA, int state, position positionB);

public:
	void printPosition(position corrent);
	STC(Map &map, position startPos);
	void printWorkingMap();
	void printDFSFunc();
	void buildWorkingMap();
	vector<position> walkingPath();
	vector<realPosition> getRealWalkingPath();
	void saveDFSToFile(const char* filePath);
	virtual ~STC();
};

#endif /* STC_H_ */
