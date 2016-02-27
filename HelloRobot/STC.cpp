
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
						<<workingMap[i][j]->neighborsList[q]->getPosition().first
						<< ","
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
	cout<<"************************"<<endl;
	cout<<"In the Walking Path!"<<endl;
	vector<position> path;
	Node *startNode = workingMap[startPosition.first][startPosition.second];
	cout<<"Start Node: "<<endl;
	startNode->printNode();
	int initialDirection;
	//up = 3;
	//down  = 1;
	//right  = 2;
	//left = 0;
	for (int i=0;i<4;i++)
	{
		cout<<startNode->edgeList[i]<<endl;
		if (startNode->edgeList[i])
		{
			cout<<"i:"<<i<<endl;
			initialDirection = i;
			break;
		}
	}
	int initialCorner = (initialDirection + 2) % 4;
	cout<<"int initialCorner = (initialDirection + 2) % 4 : "<< initialCorner<<endl;
	position initialFineGridPosition = fineGridCoordinate(startPosition, initialCorner);
	path.resize(path.size()+1);
	path[0] = map.fineToCordy(initialFineGridPosition);
	fillPath(path, startNode, initialFineGridPosition, initialCorner, initialFineGridPosition);
	return path;
}


void STC::fillPath(vector<position> &path, Node *coarseGridNode, position fineGridCoord, int corner, position initialFineGridPosition) {
	cout<<"*********************************"<<endl;
	cout<<"In the fillPath func."<<endl;
	if (fineGridCoord ==  initialFineGridPosition && path.size() > 1) {
			//this if will happen when the start node = finish node and the path size  >1 (finished the path!);
			cout<<" if fineGridCoord ==  initialFineGridPosition && path.size() > 1"<<endl;
			cout<<"we have reached the end of out path!"<<endl;
			// we've reached the end of our path
			return;
		}
		//get the edge of the working node(in the first is the start node).
		vector<bool> edges = coarseGridNode->edgeList;
		//edge[3] = up
		//edge[1] = down
		//edge[2] = right
		//edge[0] = left
		cout<<"edges[0] left : "<<edges[0]<< " edges[1]:down "<<edges[1]<< " edges[2] :right "<< edges[2]
		<< " edges[3] :up "<<edges[3]<<endl;
		//this if check if in the edges i can go down and not left.
		if (edges[(2 + corner) % 4] && !edges[(1 + corner) % 4]) {
			cout << edges[(2 + corner) % 4] <<" and "<<!edges[(1 + corner) % 4]<< endl;
			//cout<<"edges[(2 + corner) % 4: "<< edges[(2 + corner) % 4 <<endl;
			cout<<"if if (edges[(2 + corner) % 4] && !edges[(1 + corner) % 4]"<<endl;
			cout<<"go straight"<<endl;
			// go straight
			path.resize(path.size()+2);
			position firstStep;
			position secondStep;
			Node *nextNode;
			int nextCorner = corner;
			switch(corner) {
			case 0:
				// facing left
				firstStep.first = fineGridCoord.first;
				firstStep.second = fineGridCoord.second - 1;
				secondStep.first = fineGridCoord.first;
				secondStep.second = fineGridCoord.second - 2;
				nextNode = workingMap[coarseGridNode->row][coarseGridNode->col-1];
				cout<<"Facing left!"<<endl;
				printPosition(firstStep);
				printPosition(secondStep);
				break;
			case 1:
				// facing down
				firstStep.first = fineGridCoord.first + 1;
				firstStep.second = fineGridCoord.second;
				secondStep.first = fineGridCoord.first + 2;
				secondStep.second = fineGridCoord.second;
				nextNode = workingMap[coarseGridNode->row+1][coarseGridNode->col];
				cout<<"Facing down!"<<endl;
								printPosition(firstStep);
								printPosition(secondStep);
				break;
			case 2:
				// facing right
				firstStep.first = fineGridCoord.first;
				firstStep.second = fineGridCoord.second + 1;
				secondStep.first = fineGridCoord.first;
				secondStep.second = fineGridCoord.second + 2;
				nextNode = workingMap[coarseGridNode->row][coarseGridNode->col+1];
				cout<<"Facing right!"<<endl;
								printPosition(firstStep);
								printPosition(secondStep);
				break;
			case 3:
				// facing up
				firstStep.first = fineGridCoord.first -1;
				firstStep.second = fineGridCoord.second;
				secondStep.first = fineGridCoord.first - 2;
				secondStep.second = fineGridCoord.second;
				nextNode = workingMap[coarseGridNode->row-1][coarseGridNode->col];
				cout<<"Facing up!"<<endl;
								printPosition(firstStep);
								printPosition(secondStep);
				break;
			}
			path[path.size()-2] = map.fineToCordy(firstStep);
			path[path.size()-1] = map.fineToCordy(secondStep);
			fillPath(path, nextNode, secondStep, nextCorner, initialFineGridPosition);
			return;
		}

		if (edges[(1 + corner) % 4]) {
			cout<<"edges[(1 + corner) % 4] : go right"<<endl;
			// go right
			path.resize(path.size()+1);
			position firstStep;
			Node *nextNode;
			int nextCorner = (corner + 3) % 4;
			switch(corner) {
			case 0:
				// facing left
				firstStep.first = fineGridCoord.first - 1;
				firstStep.second = fineGridCoord.second;
				nextNode = workingMap[coarseGridNode->row-1][coarseGridNode->col];
				cout<<"Facing left!"<<endl;
								printPosition(firstStep);
				break;
			case 1:
				// facing down
				firstStep.first = fineGridCoord.first;
				firstStep.second = fineGridCoord.second - 1;
				nextNode = workingMap[coarseGridNode->row][coarseGridNode->col-1];
				cout<<"Facing down!"<<endl;
								printPosition(firstStep);
				break;
			case 2:
				// facing right
				firstStep.first = fineGridCoord.first + 1;
				firstStep.second = fineGridCoord.second;
				nextNode = workingMap[coarseGridNode->row+1][coarseGridNode->col];
				cout<<"Facing right!"<<endl;
								printPosition(firstStep);
				break;
			case 3:
				// facing up
				firstStep.first = fineGridCoord.first;
				firstStep.second = fineGridCoord.second + 1;
				nextNode = workingMap[coarseGridNode->row][coarseGridNode->col+1];
				cout<<"Facing up!"<<endl;
								printPosition(firstStep);
				break;
			}
			path[path.size()-1] = map.fineToCordy(firstStep);
			fillPath(path, nextNode, firstStep, nextCorner, initialFineGridPosition);
			return;
		}

		if (edges[(3 + corner) % 4] && !edges[(1 + corner) % 4]) {
			cout<<"go left: edges[(3 + corner) % 4] && !edges[(1 + corner) % 4]"<<endl;
			// go left
			path.resize(path.size()+3);
			position firstStep;
			position secondStep;
			position thirdStep;
			Node *nextNode;
			int nextCorner = (corner + 1) % 4;
			switch(corner) {
			case 0:
				// facing left
				firstStep.first = fineGridCoord.first;
				firstStep.second = fineGridCoord.second - 1;
				secondStep.first = firstStep.first + 1;
				secondStep.second = firstStep.second;
				thirdStep.first = secondStep.first + 1;
				thirdStep.second = secondStep.second;
				nextNode = workingMap[coarseGridNode->row+1][coarseGridNode->col];
				cout<<"Facing left!"<<endl;
								printPosition(firstStep);
								printPosition(secondStep);
								printPosition(thirdStep);
				break;
			case 1:
				// facing down
				firstStep.first = fineGridCoord.first + 1;
				firstStep.second = fineGridCoord.second;
				secondStep.first = firstStep.first;
				secondStep.second = firstStep.second + 1;
				thirdStep.first = secondStep.first;
				thirdStep.second = secondStep.second + 1;
				nextNode = workingMap[coarseGridNode->row][coarseGridNode->col+1];
				cout<<"Facing down!"<<endl;
												printPosition(firstStep);
												printPosition(secondStep);
												printPosition(thirdStep);
				break;
			case 2:
				// facing right
				firstStep.first = fineGridCoord.first;
				firstStep.second = fineGridCoord.second + 1;
				secondStep.first = firstStep.first - 1;
				secondStep.second = firstStep.second;
				thirdStep.first = secondStep.first - 1;
				thirdStep.second = secondStep.second;
				nextNode = workingMap[coarseGridNode->row-1][coarseGridNode->col];
				cout<<"Facing right!"<<endl;
												printPosition(firstStep);
												printPosition(secondStep);
												printPosition(thirdStep);
				break;
			case 3:
				// facing up
				firstStep.first = fineGridCoord.first -1;
				firstStep.second = fineGridCoord.second;
				secondStep.first = firstStep.first;
				secondStep.second = firstStep.second - 1;
				thirdStep.first = secondStep.first;
				thirdStep.second = secondStep.second - 1;
				nextNode = workingMap[coarseGridNode->row][coarseGridNode->col-1];
				cout<<"Facing up!"<<endl;
												printPosition(firstStep);
												printPosition(secondStep);
												printPosition(thirdStep);
				break;
			}
			path[path.size()-3] = map.fineToCordy(firstStep);
			path[path.size()-2] = map.fineToCordy(secondStep);
			path[path.size()-1] = map.fineToCordy(thirdStep);
			fillPath(path, nextNode, thirdStep, nextCorner, initialFineGridPosition);
			return;
		}

		if (edges[(0 + corner) % 4] && !edges[(1 + corner) % 4] && !edges[(2 + corner) % 4] && !edges[(3 + corner) % 4]) {
			cout<<" u turn edges[(0 + corner) % 4] && !edges[(1 + corner) % 4] && !edges[(2 + corner) % 4] && !edges[(3 + corner) % 4]"<<endl;
			// u turn
			path.resize(path.size()+4);
			position firstStep;
			position secondStep;
			position thirdStep;
			position fourthStep;
			Node *nextNode;
			int nextCorner = (corner + 2) % 4;
			switch(corner) {
			case 0:
				// facing left
				firstStep.first = fineGridCoord.first;
				firstStep.second = fineGridCoord.second - 1;
				secondStep.first = firstStep.first + 1;
				secondStep.second = firstStep.second;
				thirdStep.first = secondStep.first;
				thirdStep.second = secondStep.second + 1;
				fourthStep.first = thirdStep.first;
				fourthStep.second = thirdStep.second + 1;
				nextNode = workingMap[coarseGridNode->row][coarseGridNode->col+1];
				cout<<"Facing left!"<<endl;
												printPosition(firstStep);
												printPosition(secondStep);
												printPosition(thirdStep);
												printPosition(fourthStep);
				break;
			case 1:
				// facing down
				firstStep.first = fineGridCoord.first + 1;
				firstStep.second = fineGridCoord.second;
				secondStep.first = firstStep.first;
				secondStep.second = firstStep.second + 1;
				thirdStep.first = secondStep.first - 1;
				thirdStep.second = secondStep.second;
				fourthStep.first = thirdStep.first - 1;
				fourthStep.second = thirdStep.second;
				nextNode = workingMap[coarseGridNode->row-1][coarseGridNode->col];
				cout<<"Facing down!"<<endl;
																printPosition(firstStep);
																printPosition(secondStep);
																printPosition(thirdStep);
																printPosition(fourthStep);
				break;
			case 2:
				// facing right
				firstStep.first = fineGridCoord.first;
				firstStep.second = fineGridCoord.second + 1;
				secondStep.first = firstStep.first - 1;
				secondStep.second = firstStep.second;
				thirdStep.first = secondStep.first;
				thirdStep.second = secondStep.second - 1;
				fourthStep.first = thirdStep.first;
				fourthStep.second = thirdStep.second - 1;
				nextNode = workingMap[coarseGridNode->row][coarseGridNode->col-1];
				cout<<"Facing right!"<<endl;
																printPosition(firstStep);
																printPosition(secondStep);
																printPosition(thirdStep);
																printPosition(fourthStep);
				break;
			case 3:
				// facing up
				firstStep.first = fineGridCoord.first -1;
				firstStep.second = fineGridCoord.second;
				secondStep.first = firstStep.first;
				secondStep.second = firstStep.second - 1;
				thirdStep.first = secondStep.first + 1;
				thirdStep.second = secondStep.second;
				fourthStep.first = thirdStep.first + 1;
				fourthStep.second = thirdStep.second;
				nextNode = workingMap[coarseGridNode->row+1][coarseGridNode->col];
				cout<<"Facing up!"<<endl;
																printPosition(firstStep);
																printPosition(secondStep);
																printPosition(thirdStep);
																printPosition(fourthStep);
				break;
			}
			path[path.size()-4] = map.fineToCordy(firstStep);
			path[path.size()-3] = map.fineToCordy(secondStep);
			path[path.size()-2] = map.fineToCordy(thirdStep);
			path[path.size()-1] = map.fineToCordy(fourthStep);
			fillPath(path, nextNode, fourthStep, nextCorner, initialFineGridPosition);
			return;
		}
		cout<<"Finished!"<<endl;
}

vector<realPosition> STC::getRealWalkingPath(){
	//convert from Position to Real Position.
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




position STC::fineGridCoordinate(position coarseGridCoord, int corner) {
	cout<<"**********************************************************************"<<endl;
	cout<<"Staring fineGridCoordinate func with params: position coarseGridCoord "
			<<" ("<<coarseGridCoord.first<<" , "<<coarseGridCoord.second
			<<")"<<" int corner "<<corner<<endl;
	position fineGridCoord(coarseGridCoord.first *2, coarseGridCoord.second *2);
	cout<<"x: "<<fineGridCoord.first<<"y: "<<fineGridCoord.second<<endl;
	switch (corner)
	{
	case 0: // upper right
		fineGridCoord.second ++;
		break;
	case 1: // upper left
		// doing nothing
		break;
	case 2: // bottom left
		fineGridCoord.first ++;
		break;
	case 3://bottom right
		fineGridCoord.first ++;
		fineGridCoord.second ++;
		break;
	}
	cout<<"**********************************************"<<endl;
	return fineGridCoord;
}

void STC::DFS(Node* currentNode)
{
	currentNode->visited = true;
	int x_nodePosition = currentNode->getPosition().first;
	int y_nodePosition = currentNode->getPosition().second;
	//up = 3;
	//down  = 1;
	//right  = 2;
	//left = 0;

	//edge[3] = up
	//edge[1] = down
	//edge[2] = right
	//edge[0] = left

	//
	int row = x_nodePosition - 1;
	int col = y_nodePosition;
	if (row >= 0 && row < workingMap.size() &&col < workingMap[0].size()&& col >= 0) {
		if (workingMap[row][col] != NULL && !(workingMap[row][col]->visited)) {
			currentNode->neighborsList[1] = workingMap[row][col];
			currentNode->edgeList[1] = true;
			currentNode->neighborsList[1]->edgeList[3] = true;
			DFS(workingMap[row][col]);
		}
	}
	// down
	row = x_nodePosition + 1;
	col = y_nodePosition;
	if (row >= 0 && row < workingMap.size() &&col < workingMap[0].size()&& col >= 0) {
		if (workingMap[row][col] != NULL && !workingMap[row][col]->visited) {
			currentNode->neighborsList[3] = workingMap[row][col];
			currentNode->edgeList[3] = true;
			currentNode->neighborsList[3]->edgeList[1] = true;
			DFS(workingMap[row][col]);
		}
	}

	// right
	row = x_nodePosition;
	col = y_nodePosition + 1;
	if (row >= 0 && row < workingMap.size() &&col < workingMap[0].size()&& col >= 0) {
		if (workingMap[row][col] != NULL && !workingMap[row][col]->visited) {
			currentNode->neighborsList[0] = workingMap[row][col];
			currentNode->edgeList[0] = true;
			currentNode->neighborsList[0]->edgeList[2] = true;
			DFS(workingMap[row][col]);
		}
	}
	// left
	row = x_nodePosition;
	col = y_nodePosition - 1;
	if (row >= 0 && row < workingMap.size() &&col < workingMap[0].size()&& col >= 0) {
		if (workingMap[row][col] != NULL && !workingMap[row][col]->visited) {
			currentNode->neighborsList[2] = workingMap[row][col];
			currentNode->edgeList[2] = true;
			currentNode->neighborsList[2]->edgeList[0] = true;
			DFS(workingMap[row][col]);
		}
	}

}


STC::~STC() {}

