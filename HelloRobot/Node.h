/*
 * Node.h
 *
 *  Created on: Nov 18, 2015
 *      Author: colman
 */

#ifndef NODE_H_
#define NODE_H_

#include <vector>
using namespace std;

typedef pair<int, int> Position;

class Node {
private:



public:
	int row, col;
	bool visited;
	vector<Node *> neighborsInSpanningTree;
	vector<bool> edgesToColor;
	Node(int row, int col);
	virtual ~Node();
	Position getPosition();
	bool isVisited();
};

#endif /* NODE_H_ */
