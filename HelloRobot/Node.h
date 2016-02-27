
#ifndef NODE_H_
#define NODE_H_

#include <vector>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
using namespace std;

typedef pair<int, int> position;

class Node {
private:

public:
	position printPosition();
	Node(int row, int col);
	int row, col;
	vector<Node *> neighborsList;
	vector<bool> edgeList;
	position getPosition();
	bool visited;
	bool isVisited();
	void printNode();
	virtual ~Node();

};

#endif /* NODE_H_ */
