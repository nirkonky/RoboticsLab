/*
 * STC.cpp
 *
 *  Created on: Dec 9, 2015
 *      Author: colman
 */

#include "STC.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "lodepng.h"

using namespace std;

STC::STC(Map &map, Position startPos):map(map),startPos(startPos) {

}

void STC::buildGraph() {
	Grid coarseGrid = map.getCoarseGrid();
	int gridRows = coarseGrid.size();
	int gridCols = coarseGrid[0].size();


	graph.resize(gridRows);
	for (int i = 0; i < gridRows; i++)
		graph[i].resize(gridCols);

	for (int i=0;i<gridRows;i++) {
		for (int j=0;j<gridCols;j++) {
			if (!coarseGrid[i][j]) {
				Node* node = new Node(i, j);
				graph[i][j] = node;
			}
		}
	}
	DFS(graph[startPos.first][startPos.second]);
}

void STC::printGraph() {
	int gridRows = graph.size();
		int gridCols = graph[0].size();


		for (int i=0;i<gridRows;i++) {
			for (int j=0;j<gridCols;j++) {
				if (graph[i][j]) {
					cout << setw(2) << i << ":" << setw(2) <<  j << " ";
				} else {
					cout << "  :   ";
				}
			}
			cout << endl;
		}
}

void STC::printDFS() {
	int gridRows = graph.size();
	int gridCols = graph[0].size();

	for (int i=0;i<gridRows;i++) {
		for (int j=0;j<gridCols;j++) {
			if (graph[i][j] != NULL) {
				for (int k=0;k<4;k++) {
					if (graph[i][j]->neighborsInSpanningTree[k] != NULL) {
						cout << "(" << graph[i][j]->getPosition().first << "," << graph[i][j]->getPosition().second << ")";
						cout << " -> ";
						cout << "(" << graph[i][j]->neighborsInSpanningTree[k]->getPosition().first << "," << graph[i][j]->neighborsInSpanningTree[k]->getPosition().second << ")" << endl;
					}
				}

			}
		}
	}
}

void STC::drawLineOnImage(vector<unsigned char> &image, Position positionA, Position positionB, int mapWidth) {
	Position pixelPositionA = map.coarseToPixelCoordinate(positionA);
	Position pixelPositionB = map.coarseToPixelCoordinate(positionB);

	if (positionA.first == positionB.first) {
		// horizontal line
		int start = (pixelPositionA.second < pixelPositionB.second) ? pixelPositionA.second : pixelPositionB.second;
		int length = abs(pixelPositionA.second - pixelPositionB.second);
		for (int i = start; i <= start+length; i++) {
			int c = (pixelPositionA.first * mapWidth + i) * 4;
			image[c] = 255;
			image[c + 1] = 0;
			image[c + 2] = 0;
			image[c + 3] = 255;
		}
	} else {
		// vertical line
		int start = (pixelPositionA.first < pixelPositionB.first) ? pixelPositionA.first : pixelPositionB.first;
		int length = abs(pixelPositionA.first - pixelPositionB.first);
		for (int i = start; i <= start+length; i++) {
			int c = (i * mapWidth + pixelPositionA.second) * 4;
			image[c] = 255;
			image[c + 1] = 0;
			image[c + 2] = 0;
			image[c + 3] = 255;
		}
	}
}

vector<Position> STC::path() {
	vector<Position> path;
	Node *startNode = graph[startPos.first][startPos.second];
	int initialDirection;
	for (int i=0;i<4;i++) {
		if (startNode->edgesToColor[i]) {
			initialDirection = i;
			break;
		}
	}
	int initialCorner = (initialDirection + 2) % 4;

	Position initialFineGridPosition = fineGridCoordinate(startPos, initialCorner);
	path.resize(path.size()+1);

	path[0] = map.fineToPixelCoordinate(initialFineGridPosition);


	fillPath(path, startNode, initialFineGridPosition, initialCorner, initialFineGridPosition);
	cout<<"im here!"<<endl;
	return path;
}

void STC::drawSpanningTree(vector<unsigned char> &image, int mapWidth) {
	int gridRows = graph.size();
	int gridCols = graph[0].size();

	for (int i = 0; i < gridRows; i++) {
		for (int j = 0; j < gridCols; j++) {
			if (graph[i][j] != NULL) {
				for (int k = 0; k < 4; k++) {
					if (graph[i][j]->neighborsInSpanningTree[k] != NULL) {
						Position positionA = graph[i][j]->getPosition();
						Position positionB = graph[i][j]->neighborsInSpanningTree[k]->getPosition();
						drawLineOnImage(image, positionA, positionB, mapWidth);
					}
				}

			}
		}
	}
}

void STC::saveSpanningTreeToFile(const char* filePath) {
	Grid mapGrid = map.getMapGrid();
	vector<unsigned char> image;
	cout<<mapGrid.size()<<" ;" <<mapGrid[0].size()<<endl;
	int mapHeight = mapGrid.size();
	int mapWidth = mapGrid[0].size();

	image.resize(mapHeight * mapWidth * 4);

	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			int c = (i * mapWidth + j) * 4;
			if (mapGrid[i][j]) {
				image[c] = 0;
				image[c + 1] = 0;
				image[c + 2] = 0;
				image[c + 3] = 255;
			} else {
				image[c] = 255;
				image[c + 1] = 255;
				image[c + 2] = 255;
				image[c + 3] = 255;
			}

		}
	}

	drawSpanningTree(image, mapWidth);
	lodepng::encode(filePath, image, mapWidth, mapHeight);
}


void STC::fillPath(vector<Position> &path, Node *coarseGridNode, Position fineGridCoord, int corner, Position initialFineGridPosition) {
	if (fineGridCoord ==  initialFineGridPosition && path.size() > 1) {

		// we've reached the end of our path
		cout<<"im here"<<endl;

		return;
	}
	cout<<"Iteration"<<corner<<endl;
	vector<bool> edges = coarseGridNode->edgesToColor;
	if (edges[(2 + corner) % 4] && !edges[(1 + corner) % 4])
	{
		cout<<"in the first If edges[(2 + corner) % 4] && !edges[(1 + corner) % 4]"<<endl;
		// go straight
		path.resize(path.size()+2);
		Position firstStep;
		Position secondStep;
		Node *nextNode;
		int nextCorner = corner;
		switch(corner) {
		case 0:
			cout<<"case 0"<<endl;
			// facing left
			firstStep.first = fineGridCoord.first;
			firstStep.second = fineGridCoord.second - 1;
			secondStep.first = fineGridCoord.first;
			secondStep.second = fineGridCoord.second - 2;
			nextNode = graph[coarseGridNode->row][coarseGridNode->col-1];
			break;
		case 1:
			cout<<"case 1"<<endl;
			// facing down
			firstStep.first = fineGridCoord.first + 1;
			firstStep.second = fineGridCoord.second;
			secondStep.first = fineGridCoord.first + 2;
			secondStep.second = fineGridCoord.second;
			nextNode = graph[coarseGridNode->row+1][coarseGridNode->col];
			break;
		case 2:
			cout<<"case 2"<<endl;
			// facing right
			firstStep.first = fineGridCoord.first;
			firstStep.second = fineGridCoord.second + 1;
			secondStep.first = fineGridCoord.first;
			secondStep.second = fineGridCoord.second + 2;
			nextNode = graph[coarseGridNode->row][coarseGridNode->col+1];
			break;
		case 3:
			cout<<"case 3"<<endl;
			// facing up
			firstStep.first = fineGridCoord.first -1;
			firstStep.second = fineGridCoord.second;
			secondStep.first = fineGridCoord.first - 2;
			secondStep.second = fineGridCoord.second;
			nextNode = graph[coarseGridNode->row-1][coarseGridNode->col];
			break;
		}
		path[path.size()-2] = map.fineToPixelCoordinate(firstStep);
		path[path.size()-1] = map.fineToPixelCoordinate(secondStep);
		fillPath(path, nextNode, secondStep, nextCorner, initialFineGridPosition);
		cout<<"im here"<<endl;
		return;
	}
	cout<<"after the first IF!"<<endl;
	if (edges[(1 + corner) % 4])
	{
		// go right
		cout<< "IF edges[(1 + corner) % 4]"<<endl;
		path.resize(path.size()+1);
		Position firstStep;
		Node *nextNode;
		int nextCorner = (corner + 3) % 4;
		switch(corner) {
		case 0:
			cout<<"in case 0"<<endl;
			// facing left
			firstStep.first = fineGridCoord.first - 1;
			firstStep.second = fineGridCoord.second;
			nextNode = graph[coarseGridNode->row-1][coarseGridNode->col];
			break;
		case 1:
			cout<<"in case 1"<<endl;
			// facing down
			firstStep.first = fineGridCoord.first;
			firstStep.second = fineGridCoord.second - 1;
			nextNode = graph[coarseGridNode->row][coarseGridNode->col-1];
			break;
		case 2:
			cout<<"in case 2"<<endl;
			// facing right
			firstStep.first = fineGridCoord.first + 1;
			firstStep.second = fineGridCoord.second;
			nextNode = graph[coarseGridNode->row+1][coarseGridNode->col];
			break;
		case 3:
			cout<<"in case 3"<<endl;
			// facing up
			firstStep.first = fineGridCoord.first;
			firstStep.second = fineGridCoord.second + 1;
			nextNode = graph[coarseGridNode->row][coarseGridNode->col+1];
			break;
		}
		path[path.size()-1] = map.fineToPixelCoordinate(firstStep);
		cout<<"dont with the If!"<<endl;
		fillPath(path, nextNode, firstStep, nextCorner, initialFineGridPosition);
		cout<<"im here"<<endl;

		return;
	}
	cout<<"after the second IF!"<<endl;

	if (edges[(3 + corner) % 4] && !edges[(1 + corner) % 4])
	{
		cout<<"IF edges[(3 + corner) % 4] && !edges[(1 + corner) % 4]"<<endl;
		// go left
		path.resize(path.size()+3);
		Position firstStep;
		Position secondStep;
		Position thirdStep;
		Node *nextNode;
		int nextCorner = (corner + 1) % 4;
		switch(corner) {
		case 0:
			cout<<"case 0"<<endl;
			// facing left
			firstStep.first = fineGridCoord.first;
			firstStep.second = fineGridCoord.second - 1;
			secondStep.first = firstStep.first + 1;
			secondStep.second = firstStep.second;
			thirdStep.first = secondStep.first + 1;
			thirdStep.second = secondStep.second;
			nextNode = graph[coarseGridNode->row+1][coarseGridNode->col];
			break;
		case 1:
			cout<<"case 1"<<endl;
			// facing down
			firstStep.first = fineGridCoord.first + 1;
			firstStep.second = fineGridCoord.second;
			secondStep.first = firstStep.first;
			secondStep.second = firstStep.second + 1;
			thirdStep.first = secondStep.first;
			thirdStep.second = secondStep.second + 1;
			nextNode = graph[coarseGridNode->row][coarseGridNode->col+1];
			break;
		case 2:
			cout<<"case 2"<<endl;
			// facing right
			firstStep.first = fineGridCoord.first;
			firstStep.second = fineGridCoord.second + 1;
			secondStep.first = firstStep.first - 1;
			secondStep.second = firstStep.second;
			thirdStep.first = secondStep.first - 1;
			thirdStep.second = secondStep.second;
			nextNode = graph[coarseGridNode->row-1][coarseGridNode->col];
			break;
		case 3:
			cout<<"case 3"<<endl;
			// facing up
			firstStep.first = fineGridCoord.first -1;
			firstStep.second = fineGridCoord.second;
			secondStep.first = firstStep.first;
			secondStep.second = firstStep.second - 1;
			thirdStep.first = secondStep.first;
			thirdStep.second = secondStep.second - 1;
			nextNode = graph[coarseGridNode->row][coarseGridNode->col-1];
			break;
		}
		cout<<"done with the IF!"<<endl;
		path[path.size()-3] = map.fineToPixelCoordinate(firstStep);
		path[path.size()-2] = map.fineToPixelCoordinate(secondStep);
		path[path.size()-1] = map.fineToPixelCoordinate(thirdStep);
		fillPath(path, nextNode, thirdStep, nextCorner, initialFineGridPosition);
		cout<<"im here"<<endl;

		return;
	}
	cout<<"after IF 3!"<<endl;
	if (edges[(0 + corner) % 4] && !edges[(1 + corner) % 4] && !edges[(2 + corner) % 4] && !edges[(3 + corner) % 4])
	{
		cout<<"edges[(1 + corner) % 4] && !edges[(2 + corner) % 4!edges[(3 + corner) % 4])"<<endl;
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
			cout<<"case 0"<<endl;
			// facing left
			firstStep.first = fineGridCoord.first;
			firstStep.second = fineGridCoord.second - 1;
			secondStep.first = firstStep.first + 1;
			secondStep.second = firstStep.second;
			thirdStep.first = secondStep.first;
			thirdStep.second = secondStep.second + 1;
			fourthStep.first = thirdStep.first;
			fourthStep.second = thirdStep.second + 1;
			nextNode = graph[coarseGridNode->row][coarseGridNode->col+1];
			break;
		case 1:
			cout<<"case 1"<<endl;
			// facing down
			firstStep.first = fineGridCoord.first + 1;
			firstStep.second = fineGridCoord.second;
			secondStep.first = firstStep.first;
			secondStep.second = firstStep.second + 1;
			thirdStep.first = secondStep.first - 1;
			thirdStep.second = secondStep.second;
			fourthStep.first = thirdStep.first - 1;
			fourthStep.second = thirdStep.second;
			nextNode = graph[coarseGridNode->row-1][coarseGridNode->col];
			break;
		case 2:
			cout<<"case 2"<<endl;
			// facing right
			firstStep.first = fineGridCoord.first;
			firstStep.second = fineGridCoord.second + 1;
			secondStep.first = firstStep.first - 1;
			secondStep.second = firstStep.second;
			thirdStep.first = secondStep.first;
			thirdStep.second = secondStep.second - 1;
			fourthStep.first = thirdStep.first;
			fourthStep.second = thirdStep.second - 1;
			nextNode = graph[coarseGridNode->row][coarseGridNode->col-1];
			break;
		case 3:
			cout<<"case 3"<<endl;
			// facing up
			firstStep.first = fineGridCoord.first -1;
			firstStep.second = fineGridCoord.second;
			secondStep.first = firstStep.first;
			secondStep.second = firstStep.second - 1;
			thirdStep.first = secondStep.first + 1;
			thirdStep.second = secondStep.second;
			fourthStep.first = thirdStep.first + 1;
			fourthStep.second = thirdStep.second;
			nextNode = graph[coarseGridNode->row+1][coarseGridNode->col];
			break;
		}
		cout<<"finished with the iF!"<<endl;
		path[path.size()-4] = map.fineToPixelCoordinate(firstStep);
		path[path.size()-3] = map.fineToPixelCoordinate(secondStep);
		path[path.size()-2] = map.fineToPixelCoordinate(thirdStep);
		path[path.size()-1] = map.fineToPixelCoordinate(fourthStep);
		fillPath(path, nextNode, fourthStep, nextCorner, initialFineGridPosition);
		cout<<"im here"<<endl;

		return;
	}
	cout<<"im here!"<<endl;
	return;
}

vector<realPosition> STC::realPath(){
	vector<Position> pixelPath = path();
	vector<realPosition> realPath;
	realPath.resize(pixelPath.size());
	for (int i = 0; i < pixelPath.size(); ++i) {
		realPath[i] = map.pixelToRobotPosition(pixelPath[i]);
		cout<<realPath[i].first<<","<<realPath[i].second<<endl;
	}

	return realPath;
}

void STC::savePathToFile(vector<realPosition> path, const char* filePath) {
	Grid mapGrid = map.getMapGrid();
	vector<unsigned char> image;

	int mapHeight = mapGrid.size();
	int mapWidth = mapGrid[0].size();

	image.resize(mapHeight * mapWidth * 4);

	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			int c = (i * mapWidth + j) * 4;
			if (mapGrid[i][j]) {
				image[c] = 0;
				image[c + 1] = 0;
				image[c + 2] = 0;
				image[c + 3] = 255;
			} else {
				image[c] = 255;
				image[c + 1] = 255;
				image[c + 2] = 255;
				image[c + 3] = 255;
			}

		}
	}
	drawSpanningTree(image, mapWidth);
	for (int i=0;i<path.size();i++) {
		Coordinate pixelCoord = path[i];
		int c = (pixelCoord.first * mapWidth + pixelCoord.second) * 4;
		image[c] = 0;
		image[c + 1] = 0;
		image[c + 2] = 255;
		image[c + 3] = 255;
	}
	cout<<"imhere!"<<endl;
	lodepng::encode(filePath, image, mapWidth, mapHeight);
}


Position STC::fineGridCoordinate(Position coarseGridCoord, int corner) {
	Position fineGridCoord(coarseGridCoord.first * 2, coarseGridCoord.second * 2);
	switch (corner) {
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

void STC::DFS(Node* n) {
	n->visited = true;
	int x = n->getPosition().first;
	int y = n->getPosition().second;

	// right
	int row = x;
	int col = y + 1;
	if (row >= 0 && row < graph.size() && col >= 0 && col < graph[0].size()) {
		if (graph[row][col] != NULL && !graph[row][col]->visited) {
			n->neighborsInSpanningTree[0] = graph[row][col];
			n->edgesToColor[0] = true;
			n->neighborsInSpanningTree[0]->edgesToColor[2] = true;
			DFS(graph[row][col]);
		}
	}

	// up
	row = x - 1;
	col = y;
	if (row >= 0 && row < graph.size() && col >= 0 && col < graph[0].size()) {
		if (graph[row][col] != NULL && !graph[row][col]->visited) {
			n->neighborsInSpanningTree[1] = graph[row][col];
			n->edgesToColor[1] = true;
			n->neighborsInSpanningTree[1]->edgesToColor[3] = true;
			DFS(graph[row][col]);
		}
	}

	// left
	row = x;
	col = y - 1;
	if (row >= 0 && row < graph.size() && col >= 0 && col < graph[0].size()) {
		if (graph[row][col] != NULL && !graph[row][col]->visited) {
			n->neighborsInSpanningTree[2] = graph[row][col];
			n->edgesToColor[2] = true;
			n->neighborsInSpanningTree[2]->edgesToColor[0] = true;
			DFS(graph[row][col]);
		}
	}

	// down
	row = x + 1;
	col = y;
	if (row >= 0 && row < graph.size() && col >= 0 && col < graph[0].size()) {
		if (graph[row][col] != NULL && !graph[row][col]->visited) {
			n->neighborsInSpanningTree[3] = graph[row][col];
			n->edgesToColor[3] = true;
			n->neighborsInSpanningTree[3]->edgesToColor[1] = true;
			DFS(graph[row][col]);
		}
	}
}


STC::~STC() {
	// TODO Auto-generated destructor stub
}

