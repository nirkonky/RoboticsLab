
#include "STC.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "lodepng.h"
#include "defines.h"

using namespace std;

STC::STC(Map &map, position startPos):map(map),startPosition(startPos) {}

void STC::buildWorkingMap() {
	Grid coarseGrid = map.getCoarseGrid();
	workingMap.resize(coarseGrid.size());
	for (int i = 0; i < coarseGrid.size(); i++)
		workingMap[i].resize(coarseGrid[0].size());

	for (int i=0;i<coarseGrid.size();i++)
	{
		for (int j=0;j<coarseGrid[0].size();j++)
		{
			if (!coarseGrid[i][j])
			{
				Node* node = new Node(i, j);
				workingMap[i][j] = node;
			}
		}
	}
	DFS(workingMap[startPosition.first][startPosition.second]);
}

void STC::printPosition(position corrent){
	cout<<" ("<<corrent.first <<","<<corrent.second<<") "<<endl;
}

void STC::printWorkingMap() {
	for (int i=0;i<workingMap.size();i++)
	{
		for (int j=0;j<workingMap[0].size();j++)
		{
			if (workingMap[i][j])
			{
				cout << setw(2) << i << ":" << setw(2) <<  j << "  ";
			}
			else
			{
				cout << "  :   ";
			}
		}
		cout << endl;
	}
}

void STC::printDFSFunc() {
	for (int i=0;i<workingMap.size();i++)
	{
		for (int j=0;j<workingMap[0].size();j++)
		{
			if (workingMap[i][j] != NULL)
			{
				//move on all hes Neighbors if any.
				for (int q=0;q<4;q++)
				{
					if (workingMap[i][j]->neighborsList[q] != NULL)
					{
						cout << workingMap[i][j]->getPosition().first
						<<"," << workingMap[i][j]->getPosition().second<< " --> "
						<<workingMap[i][j]->neighborsList[q]->getPosition().first<< ","
						<<workingMap[i][j]->neighborsList[q]->getPosition().second
						<<endl;
					}
				}
			}
		}
	}
}


void STC::saveDFSToFile(const char* filePath) {
	Grid mapGrid = map.getMapGrid();
	vector<unsigned char> image;

	image.resize(mapGrid.size() * mapGrid[0].size() * 4);

	for (int i = 0; i < mapGrid.size(); i++)
	{
		for (int j = 0; j < mapGrid[0].size(); j++)
		{
			int c = (i * mapGrid[0].size() + j) * 4;
			if (mapGrid[i][j])
			{
				image[c] = 0;
				image[c + 1] = 0;
				image[c + 2] = 0;
				image[c + 3] = 255;
			}
			else
			{
				image[c] = 255;
				image[c + 1] = 255;
				image[c + 2] = 255;
				image[c + 3] = 255;
			}
		}
	}
	int gridRows = workingMap.size();
		int gridCols = workingMap[0].size();

		for (int i = 0; i < gridRows; i++)
		{
			for (int j = 0; j < gridCols; j++)
			{
				if (workingMap[i][j] != NULL)
				{
					for (int k = 0; k < 4; k++)
					{
						if (workingMap[i][j]->neighborsList[k] != NULL)
						{
							position posA = workingMap[i][j]->getPosition();
							position posB = workingMap[i][j]->neighborsList[k]->getPosition();
							position pixPosOne = map.coarseToCordy(posA);
							position pixPosTwo = map.coarseToCordy(posB);
							if (posA.first == posB.first)
							{
								int start = 0;
								if (pixPosOne.second < pixPosTwo.second)
								{
									start = pixPosOne.second;
								}
								else
								{
									start = pixPosTwo.second;
								}
								int length = abs(pixPosOne.second - pixPosTwo.second);
								for (int i = start; i <= start+length; i++)
								{
									int c = (pixPosOne.first * mapGrid[0].size() + i) * 4;
									image[c] = 255;
									image[c + 1] = 0;
									image[c + 2] = 0;
									image[c + 3] = 255;
								}
							}
							else
							{
								int start = 0;
								if (pixPosOne.second < pixPosTwo.second)
								{
									start = pixPosOne.first;
								}
								else
								{
									start = pixPosTwo.first;
								}
								int length = abs(pixPosOne.first - pixPosTwo.first);
								for (int i = start; i <= start+length; i++)
								{
									int c = (i * mapGrid[0].size() + pixPosOne.second) * 4;
									image[c] = 255;
									image[c + 1] = 0;
									image[c + 2] = 0;
									image[c + 3] = 255;
								}
							}
						}
					}
				}
			}
		}

	lodepng::encode(filePath, image, mapGrid[0].size(), mapGrid.size());
}


vector<position> STC::walkingPath() {
	vector<position> path;
	Node *startNode = workingMap[startPosition.first][startPosition.second];
	startNode->printNode();
	int initialDirection;
	int modulo = 4;
	for (int i=0;i<modulo;i++)
	{
		if (startNode->edgeList[i])
		{
			cout<<"i:"<<i<<endl;
			initialDirection = i;
			break;
		}
	}
	int initialCorner = (initialDirection + 2) % modulo;
	position fineGridCoord(startPosition.first *2, startPosition.second *2);
	if (initialCorner == 0)
	{
		fineGridCoord.second ++;
	}
	else if(initialCorner == 2)
	{
		fineGridCoord.first ++;
	}
	else if(initialCorner == 3)
	{
		fineGridCoord.second ++;
		fineGridCoord.first ++;
	}
	path.resize(path.size()+1);
	path[0] = map.fineToCordy(fineGridCoord);
	fillPath(path, startNode, fineGridCoord, initialCorner, fineGridCoord);
	return path;
}


void STC::fillPath(vector<position> &path, Node *currentNode, position positionA, int next, position positionB) {
	int modulo = 4;
	if (positionA ==  positionB && path.size() > 1) {
			//this if will happen when the start node = finish node and the path size  >1 (finished the path!);
			cout<<"we have reached the end of out path!"<<endl;
			return;
		}
		int tables;
		Node *nextNode;
		int row,col;
		int nextCorner;
		position moveA,moveB,moveC,moveD;
		//get the edge of the working node(in the first is the start node).
		vector<bool> edges = currentNode->edgeList;
		if (edges[(2 + next) % modulo] && !edges[(1 + next) % modulo])
		{
			// go straight
			nextCorner = next;
			switch(next)
			{
			case 0:
				// facing left
				moveA.first = positionA.first;
				moveA.second = positionA.second - 1;
				moveB.first = positionA.first;
				moveB.second = positionA.second - 2;
				row = currentNode->row;
				col = currentNode->col-1;
				cout<<"Facing left!"<<endl;
				break;
			case 1:
				// facing up
				moveA.first = positionA.first + 1;
				moveA.second = positionA.second;
				moveB.first = positionA.first + 2;
				moveB.second = positionA.second;
				row = currentNode->row+1;
				col = currentNode->col;
				break;
			case 2:
				// facing right
				moveA.first = positionA.first;
				moveA.second = positionA.second + 1;
				moveB.first = positionA.first;
				moveB.second = positionA.second + 2;
				row = currentNode->row;
				col = currentNode->col+1;
				break;
			case 3:
				// facing down
				moveA.first = positionA.first -1;
				moveA.second = positionA.second;
				moveB.first = positionA.first - 2;
				moveB.second = positionA.second;
				row = currentNode->row-1;
				col = currentNode->col;
				break;
			}
			nextNode = workingMap[row][col];
			tables = 2;
			path.resize(path.size()+tables);
			path[path.size()-tables] = map.fineToCordy(moveA);
			path[path.size()-tables+1] = map.fineToCordy(moveB);
			fillPath(path, nextNode, moveB, nextCorner, positionB);
			return;
		}
		if (edges[(1 + next) % modulo]) {
			// go right
			nextCorner = (next + 3) % modulo;

			switch(next)
			{
			case 0:
				// facing left
				moveA.first = positionA.first - 1;
				moveA.second = positionA.second;
				row = currentNode->row-1;
				col = currentNode->col;
				break;
			case 1:
				// facing down
				moveA.first = positionA.first;
				moveA.second = positionA.second - 1;
				row = currentNode->row;
				col = currentNode->col-1;
				break;
			case 2:
				// facing right
				moveA.first = positionA.first + 1;
				moveA.second = positionA.second;
				row = currentNode->row+1;
				col = currentNode->col;
				break;
			case 3:
				// facing up
				moveA.first = positionA.first;
				moveA.second = positionA.second + 1;
				row = currentNode->row;
				col = currentNode->col+1;
				break;
			}
			nextNode = workingMap[row][col];
			tables = 1;
			path.resize(path.size()+tables);
			path[path.size()-tables] = map.fineToCordy(moveA);
			fillPath(path, nextNode, moveA, nextCorner, positionB);
			return;
		}

		if (edges[(3 + next) % modulo] && !edges[(1 + next) % modulo]) {
			cout<<"go left: edges[(3 + corner) % 4] && !edges[(1 + corner) % 4]"<<endl;
			// go left
			nextCorner = (next + 1) % modulo;
			switch(next) {
			case 0:
				// facing left
				moveA.first = positionA.first;
				moveA.second = positionA.second - 1;

				moveB.first = moveA.first + 1;
				moveB.second = moveA.second;

				moveC.first = moveB.first + 1;
				moveC.second = moveB.second;
				row = currentNode->row+1;
				col = currentNode->col;
				break;
			case 1:
				// facing up
				moveA.first = positionA.first + 1;
				moveA.second = positionA.second;

				moveB.first = moveA.first;
				moveB.second = moveA.second + 1;

				moveC.first = moveB.first;
				moveC.second = moveB.second + 1;
				row = currentNode->row;
				col = currentNode->col+1;
				break;
			case 2:
				// facing right
				moveA.first = positionA.first;
				moveA.second = positionA.second + 1;

				moveB.first = moveA.first - 1;
				moveB.second = moveA.second;

				moveC.first = moveB.first - 1;
				moveC.second = moveB.second;
				row = currentNode->row-1;
				col = currentNode->col;
				break;
			case 3:
				// facing up
				moveA.first = positionA.first -1;
				moveA.second = positionA.second;

				moveB.first = moveA.first;
				moveB.second = moveA.second - 1;

				moveC.first = moveB.first;
				moveC.second = moveB.second - 1;
				row = currentNode->row;
				col = currentNode->col-1;
				break;
			}
			nextNode = workingMap[row][col];
			tables = 3;
			path.resize(path.size()+tables);
			path[path.size()-tables] = map.fineToCordy(moveA);
			path[path.size()-tables+1] = map.fineToCordy(moveB);
			path[path.size()-tables+2] = map.fineToCordy(moveC);
			fillPath(path, nextNode, moveC, nextCorner, positionB);
			return;
		}

		if (edges[(0 + next) % modulo] && !edges[(1 + next) % modulo] && !edges[(2 + next) % modulo] && !edges[(3 + next) % modulo]) {
			// u turn
			nextCorner = (next + 2) % modulo;
			switch(next)
			{
			case 0:
				// facing left
				moveA.first = positionA.first;
				moveA.second = positionA.second - 1;

				moveB.first = moveA.first + 1;
				moveB.second = moveA.second;

				moveC.first = moveB.first;
				moveC.second = moveB.second + 1;

				moveD.first = moveC.first;
				moveD.second = moveC.second + 1;
				row = currentNode->row;
				col = currentNode->col+1;
				break;
			case 1:
				// facing down
				moveA.first = positionA.first + 1;
				moveA.second = positionA.second;

				moveB.first = moveA.first;
				moveB.second = moveA.second + 1;

				moveC.first = moveB.first - 1;
				moveC.second = moveB.second;

				moveD.first = moveC.first - 1;
				moveD.second = moveC.second;
				row = currentNode->row-1;
				col = currentNode->col;
				break;
			case 2:
				// facing right
				moveA.first = positionA.first;
				moveA.second = positionA.second + 1;

				moveB.first = moveA.first - 1;
				moveB.second = moveA.second;

				moveC.first = moveB.first;
				moveC.second = moveB.second - 1;

				moveD.first = moveC.first;
				moveD.second = moveC.second - 1;
				row = currentNode->row;
				col = currentNode->col-1;
				break;
			case 3:
				// facing up
				moveA.first = positionA.first -1;
				moveA.second = positionA.second;

				moveB.first = moveA.first;
				moveB.second = moveA.second - 1;

				moveC.first = moveB.first + 1;
				moveC.second = moveB.second;

				moveD.first = moveC.first + 1;
				moveD.second = moveC.second;
				row = currentNode->row+1;
				col = currentNode->col;
				break;
			}
			nextNode = workingMap[row][col];
			tables = 4;
			path.resize(path.size()+tables);
			path[path.size()-tables] = map.fineToCordy(moveA);
			path[path.size()-tables+1] = map.fineToCordy(moveB);
			path[path.size()-tables+2] = map.fineToCordy(moveC);
			path[path.size()-tables+3] = map.fineToCordy(moveD);
			fillPath(path, nextNode, moveD, nextCorner, positionB);
			return;
		}
		cout<<"Finished!"<<endl;
}

vector<realPosition> STC::getRealWalkingPath(){
	//convert from Position to Real Position of the robot.
	vector<position> pixelPath = walkingPath();
	vector<realPosition> realPath;
	realPath.resize(pixelPath.size());
	realPosition robotPosition ;
	for (int i = 0; i < pixelPath.size(); ++i)
	{
		robotPosition.first = (pixelPath[i].first - 0.5 * map.mapHeight) * map.mapResolution * -1;
		robotPosition.second = (pixelPath[i].second - 0.5 * map.mapWidth) * map.mapResolution;
		realPath[i] = robotPosition;
	}
	return realPath;
}


void STC::DFS(Node* currentNode)
{
	currentNode->visited = true;
	int x_nodePosition = currentNode->getPosition().first;
	int y_nodePosition = currentNode->getPosition().second;


	// right
	int row = x_nodePosition;
	int col = y_nodePosition + 1;
	if (col < workingMap[0].size()&& col >= 0 && workingMap[row][col] != NULL && !workingMap[row][col]->visited)
	{
			currentNode->neighborsList[0] = workingMap[row][col];
			currentNode->edgeList[0] = true;
			currentNode->neighborsList[0]->edgeList[2] = true;
			DFS(workingMap[row][col]);
	}


	// down
	row = x_nodePosition + 1;
	col = y_nodePosition;
	if (row >= 0 && row < workingMap.size()&& workingMap[row][col] != NULL && !workingMap[row][col]->visited)
	{
			currentNode->neighborsList[3] = workingMap[row][col];
			currentNode->edgeList[3] = true;
			currentNode->neighborsList[3]->edgeList[1] = true;
			DFS(workingMap[row][col]);
	}
	//up
	row = x_nodePosition - 1;
	col = y_nodePosition;
	if (row >= 0 && row < workingMap.size()&& workingMap[row][col] != NULL && !(workingMap[row][col]->visited))
	{
			currentNode->neighborsList[1] = workingMap[row][col];
			currentNode->edgeList[1] = true;
			currentNode->neighborsList[1]->edgeList[3] = true;
			DFS(workingMap[row][col]);
	}


	// left
	row = x_nodePosition;
	col = y_nodePosition - 1;
	if (col < workingMap[0].size() && col >= 0&& workingMap[row][col] != NULL && !workingMap[row][col]->visited)
	{
			currentNode->neighborsList[2] = workingMap[row][col];
			currentNode->edgeList[2] = true;
			currentNode->neighborsList[2]->edgeList[0] = true;
			DFS(workingMap[row][col]);
	}

}


STC::~STC() {}

