
#include "STC.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "lodepng.h"
#include "defines.h"

using namespace std;

STC::STC(Map &map, Position startPos):map(map),startPosition(startPos) {

}

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
					if (workingMap[i][j]->neighborsInSpanningTree[q] != NULL)
					{
						cout << workingMap[i][j]->getPosition().first
						<<"," << workingMap[i][j]->getPosition().second<< " --> "
						<<workingMap[i][j]->neighborsInSpanningTree[q]->getPosition().first
						<< ","
						<<workingMap[i][j]->neighborsInSpanningTree[q]->getPosition().second
						<<endl;
					}
				}
			}
		}
	}
}

void STC::drawLineOnImage(vector<unsigned char> &image, Position positionA, Position positionB, int mapWidth) {
	Position pixelPositionA = map.coarseToPixelCoordinate(positionA);
	Position pixelPositionB = map.coarseToPixelCoordinate(positionB);

	if (positionA.first == positionB.first)
	{
		// horizontal line
		int start = (pixelPositionA.second < pixelPositionB.second) ? pixelPositionA.second : pixelPositionB.second;
		int length = abs(pixelPositionA.second - pixelPositionB.second);
		for (int i = start; i <= start+length; i++)
		{
			int c = (pixelPositionA.first * mapWidth + i) * 4;
			image[c] = 255;
			image[c + 1] = 0;
			image[c + 2] = 0;
			image[c + 3] = 255;
		}
	}
	else
	{
		// vertical line
		int start = (pixelPositionA.first < pixelPositionB.first) ? pixelPositionA.first : pixelPositionB.first;
		int length = abs(pixelPositionA.first - pixelPositionB.first);
		for (int i = start; i <= start+length; i++)
		{
			int c = (i * mapWidth + pixelPositionA.second) * 4;
			image[c] = 255;
			image[c + 1] = 0;
			image[c + 2] = 0;
			image[c + 3] = 255;
		}
	}
}

//can delete this func.
void STC::drawSpanningTree(vector<unsigned char> &image, int mapWidth) {
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
					if (workingMap[i][j]->neighborsInSpanningTree[k] != NULL)
					{
						Position positionA = workingMap[i][j]->getPosition();
						Position positionB = workingMap[i][j]->neighborsInSpanningTree[k]->getPosition();
						drawLineOnImage(image, positionA, positionB, mapWidth);
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
						if (workingMap[i][j]->neighborsInSpanningTree[k] != NULL)
						{
							Position positionA = workingMap[i][j]->getPosition();
							Position positionB = workingMap[i][j]->neighborsInSpanningTree[k]->getPosition();
							drawLineOnImage(image, positionA, positionB, mapGrid[0].size());
						}
					}
				}
			}
		}

	//drawSpanningTree(image, mapWidth);
	lodepng::encode(filePath, image, mapGrid[0].size(), mapGrid.size());
}


vector<Position> STC::walkingPath() {
	vector<Position> path;
	Node *startNode = workingMap[startPosition.first][startPosition.second];
	int initialDirection;
	for (int i=0;i<4;i++)
	{
		if (startNode->edgesToColor[i])
		{
			cout<<"i:"<<i<<endl;
			initialDirection = i;
			break;
		}
	}
	int initialCorner = (initialDirection + 2) % 4;

	Position initialFineGridPosition = fineGridCoordinate(startPosition, initialCorner);
	path.resize(path.size()+1);

	path[0] = map.fineToPixelCoordinate(initialFineGridPosition);


	fillPath(path, startNode, initialFineGridPosition, initialCorner, initialFineGridPosition);
	return path;
}


void STC::fillPath(vector<Position> &path, Node *coarseGridNode, Position fineGridCoord, int corner, Position initialFineGridPosition) {
	if (fineGridCoord ==  initialFineGridPosition && path.size() > 1) {
			// we've reached the end of our path
			return;
		}
		vector<bool> edges = coarseGridNode->edgesToColor;
		if (edges[(2 + corner) % 4] && !edges[(1 + corner) % 4]) {
			// go straight
			path.resize(path.size()+2);
			Position firstStep;
			Position secondStep;
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
				break;
			case 1:
				// facing down
				firstStep.first = fineGridCoord.first + 1;
				firstStep.second = fineGridCoord.second;
				secondStep.first = fineGridCoord.first + 2;
				secondStep.second = fineGridCoord.second;
				nextNode = workingMap[coarseGridNode->row+1][coarseGridNode->col];
				break;
			case 2:
				// facing right
				firstStep.first = fineGridCoord.first;
				firstStep.second = fineGridCoord.second + 1;
				secondStep.first = fineGridCoord.first;
				secondStep.second = fineGridCoord.second + 2;
				nextNode = workingMap[coarseGridNode->row][coarseGridNode->col+1];
				break;
			case 3:
				// facing up
				firstStep.first = fineGridCoord.first -1;
				firstStep.second = fineGridCoord.second;
				secondStep.first = fineGridCoord.first - 2;
				secondStep.second = fineGridCoord.second;
				nextNode = workingMap[coarseGridNode->row-1][coarseGridNode->col];
				break;
			}
			path[path.size()-2] = map.fineToPixelCoordinate(firstStep);
			path[path.size()-1] = map.fineToPixelCoordinate(secondStep);
			fillPath(path, nextNode, secondStep, nextCorner, initialFineGridPosition);
			return;
		}

		if (edges[(1 + corner) % 4]) {
			// go right
			path.resize(path.size()+1);
			Position firstStep;
			Node *nextNode;
			int nextCorner = (corner + 3) % 4;
			switch(corner) {
			case 0:
				// facing left
				firstStep.first = fineGridCoord.first - 1;
				firstStep.second = fineGridCoord.second;
				nextNode = workingMap[coarseGridNode->row-1][coarseGridNode->col];
				break;
			case 1:
				// facing down
				firstStep.first = fineGridCoord.first;
				firstStep.second = fineGridCoord.second - 1;
				nextNode = workingMap[coarseGridNode->row][coarseGridNode->col-1];
				break;
			case 2:
				// facing right
				firstStep.first = fineGridCoord.first + 1;
				firstStep.second = fineGridCoord.second;
				nextNode = workingMap[coarseGridNode->row+1][coarseGridNode->col];
				break;
			case 3:
				// facing up
				firstStep.first = fineGridCoord.first;
				firstStep.second = fineGridCoord.second + 1;
				nextNode = workingMap[coarseGridNode->row][coarseGridNode->col+1];
				break;
			}
			path[path.size()-1] = map.fineToPixelCoordinate(firstStep);
			fillPath(path, nextNode, firstStep, nextCorner, initialFineGridPosition);
			return;
		}

		if (edges[(3 + corner) % 4] && !edges[(1 + corner) % 4]) {
			// go left
			path.resize(path.size()+3);
			Position firstStep;
			Position secondStep;
			Position thirdStep;
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
				break;
			}
			path[path.size()-3] = map.fineToPixelCoordinate(firstStep);
			path[path.size()-2] = map.fineToPixelCoordinate(secondStep);
			path[path.size()-1] = map.fineToPixelCoordinate(thirdStep);
			fillPath(path, nextNode, thirdStep, nextCorner, initialFineGridPosition);
			return;
		}

		if (edges[(0 + corner) % 4] && !edges[(1 + corner) % 4] && !edges[(2 + corner) % 4] && !edges[(3 + corner) % 4]) {
			// u turn
			path.resize(path.size()+4);
			Position firstStep;
			Position secondStep;
			Position thirdStep;
			Position fourthStep;
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
				break;
			}
			path[path.size()-4] = map.fineToPixelCoordinate(firstStep);
			path[path.size()-3] = map.fineToPixelCoordinate(secondStep);
			path[path.size()-2] = map.fineToPixelCoordinate(thirdStep);
			path[path.size()-1] = map.fineToPixelCoordinate(fourthStep);
			fillPath(path, nextNode, fourthStep, nextCorner, initialFineGridPosition);
			return;
		}
}

vector<realPosition> STC::getRealWalkingPath(){
	//convert from Position to Real Position.
	vector<Position> pixelPath = walkingPath();
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




Position STC::fineGridCoordinate(Position coarseGridCoord, int corner) {
	cout<<"in Fine Grid Coordinate!: "<<endl;
	cout<<"("<<coarseGridCoord.first<<","<<coarseGridCoord.second<<")"<<endl;
	Position fineGridCoord(coarseGridCoord.first *2, coarseGridCoord.second *2);
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
	case 3:
		fineGridCoord.first ++;
		fineGridCoord.second ++;
		break;
	}
	return fineGridCoord;
}

void STC::DFS(Node* n)
{
	n->visited = true;
	int x = n->getPosition().first;
	int y = n->getPosition().second;



	// up
	int row = x - 1;
	int col = y;
	if (row >= 0 && row < workingMap.size() && col >= 0 && col < workingMap[0].size()) {
		if (workingMap[row][col] != NULL && !workingMap[row][col]->visited) {
			n->neighborsInSpanningTree[1] = workingMap[row][col];
			n->edgesToColor[1] = true;
			n->neighborsInSpanningTree[1]->edgesToColor[3] = true;
			DFS(workingMap[row][col]);
		}
	}
	// down
	row = x + 1;
	col = y;
	if (row >= 0 && row < workingMap.size() && col >= 0 && col < workingMap[0].size()) {
		if (workingMap[row][col] != NULL && !workingMap[row][col]->visited) {
			n->neighborsInSpanningTree[3] = workingMap[row][col];
			n->edgesToColor[3] = true;
			n->neighborsInSpanningTree[3]->edgesToColor[1] = true;
			DFS(workingMap[row][col]);
		}
	}

	// right
	row = x;
	col = y + 1;
	if (row >= 0 && row < workingMap.size() && col >= 0 && col < workingMap[0].size()) {
		if (workingMap[row][col] != NULL && !workingMap[row][col]->visited) {
			n->neighborsInSpanningTree[0] = workingMap[row][col];
			n->edgesToColor[0] = true;
			n->neighborsInSpanningTree[0]->edgesToColor[2] = true;
			DFS(workingMap[row][col]);
		}
	}
	// left
	row = x;
	col = y - 1;
	if (row >= 0 && row < workingMap.size() && col >= 0 && col < workingMap[0].size()) {
		if (workingMap[row][col] != NULL && !workingMap[row][col]->visited) {
			n->neighborsInSpanningTree[2] = workingMap[row][col];
			n->edgesToColor[2] = true;
			n->neighborsInSpanningTree[2]->edgesToColor[0] = true;
			DFS(workingMap[row][col]);
		}
	}

}


STC::~STC() {
	// TODO Auto-generated destructor stub
}

