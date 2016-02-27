
#include "Node.h"

Node::Node(int row, int col) : row(row), col(col), visited(false)
{
	neighborsList.resize(4);
	edgeList.resize(4);
	for (int i = 0; i < edgeList.size(); ++i)
	{
		edgeList[i] = false;
	}
}
position Node::printPosition(){
	cout<<" ("<<row <<","<<col<<") "<<endl;
}

position Node::getPosition()
{
	position position;
	position.first = row;
	position.second = col;
	return position;
}
void Node::printNode(){cout<<"("<<row<<","<<col<<")"<<endl;}

Node::~Node() {}

