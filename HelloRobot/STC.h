/*
 * STC.h
 *
 *  Created on: Dec 9, 2015
 *      Author: colman
 */

#ifndef STC_H_
#define STC_H_

#include "Map.h"
#include "Node.h"
#include "defines.h"
using namespace std;


class STC {
private:
	Map &map;
	vector<vector<Node *> > graph;
	Position startPos;

	void DFS(Node* n);
	void drawLineOnImage(vector<unsigned char> &image, Position positionA, Position positionB, int mapWidth);
	void drawSpanningTree(vector<unsigned char> &image, int mapWidth);
	Position fineGridCoordinate(Position coarseGridCoord, int corner);
	void fillPath(vector<Position> &path, Node *coarseGridNode, Position fineGridCoord, int corner, Position initialFineGridPosition);
public:
	STC(Map &map, Position startPos);
	void printGraph();
	void printDFS();
	void buildGraph();
	vector<Position> path();
	vector<realPosition> realPath();
	void saveSpanningTreeToFile(const char* filePath);
	void savePathToFile(vector<realPosition> path, const char* filePath);
	virtual ~STC();
};

#endif /* STC_H_ */
