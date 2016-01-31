
#include "Node.h"

Node::Node(int row, int col) : row(row), col(col), visited(false) {
	neighborsInSpanningTree.resize(4);
	edgesToColor.resize(4);
	for (int i = 0; i < edgesToColor.size(); ++i)
	{
		edgesToColor[i] = false;
	}
}

Position Node::getPosition() {
	Position position;
	position.first = row;
	position.second = col;
	return position;
}
void Node::printNode(){
	cout<<"("<<row<<","<<col<<")"<<endl;
}

Node::~Node() {
	// TODO Auto-generated destructor stub
}

