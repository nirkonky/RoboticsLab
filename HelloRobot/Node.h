
#ifndef NODE_H_
#define NODE_H_

#include <vector>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
using namespace std;

typedef pair<int, int> Position;

class Node {
private:



public:
	void printNode();
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
