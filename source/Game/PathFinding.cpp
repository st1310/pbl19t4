
#include "PathFinding.h"
#include <list>
#include "Common.h"
#include "FirstPersonCamera.h"

PathFinding::PathFinding() {
	
}

PathFinding::~PathFinding() {

}

bool PathFinding::OnUserCreate() {
	
	collider = new Colliders();
	nodes = new sNode[nMapWidth*nMapHeight];

	//grid of nodes
	for (int x = 0; x < nMapWidth; x++) {
		for (int y = 0; y < nMapHeight; y++) {
			nodes[y*nMapWidth + x].x = -180 + y*10;
			nodes[y*nMapWidth + x].y = -500 + x*10;
			nodes[y*nMapWidth + x].bObstacle = false;
			nodes[y*nMapWidth + x].bVisited = false;
			nodes[y*nMapWidth + x].parent = nullptr;
			nodes[y*nMapWidth + x].bbox = new BoundingBox(XMFLOAT3(nodes[y*nMapWidth + x].x, 63.0f, nodes[y*nMapWidth + x].y), XMFLOAT3(10.f, 10.f, 10.f));
			collider->PushNewBoundingBox(nodes[y*nMapWidth + x].bbox);

			/*if (nodes[y*nMapWidth + x].x >= -180 && nodes[y*nMapWidth + x].x <= -10 && nodes[y*nMapWidth + x].y >= -450 && nodes[y*nMapWidth + x].y <= -320) {
				nodes[y*nMapWidth + x].bObstacle = true;
			}*/
		}

		
	}

	for (int x = 0; x < (nMapWidth*nMapHeight); x++) {
		if (nodes[x].x >= -180 && nodes[x].x <= -10 && nodes[x].y >= -450 && nodes[x].y <= -320) {
			nodes[x].bObstacle = true;
		}
		if (nodes[x].x >= -180 && nodes[x].x <= 170 && nodes[x].y >= -130 && nodes[x].y <= -20) {
			nodes[x].bObstacle = true;
		}
		if (nodes[x].x >= -180 && nodes[x].x <= 170 && nodes[x].y >= 30 && nodes[x].y <= 140) {
			nodes[x].bObstacle = true;
		}
		if (nodes[x].x >= -180 && nodes[x].x <= 170 && nodes[x].y >= 30 && nodes[x].y <= 140) {
			nodes[x].bObstacle = true;
		}
		if (nodes[x].x >= 20 && nodes[x].x <= 170 && nodes[x].y >= -310 && nodes[x].y <= -20) {
			nodes[x].bObstacle = true;
		}
		if (nodes[x].x >= -40 && nodes[x].x <= 80 && nodes[x].y >= -260 && nodes[x].y <= -190) {
			nodes[x].bObstacle = true;
		}

	}

	for (int x = 0; x < nMapWidth; x++) {
		for (int y = 0; y < nMapHeight; y++) {
			if (y > 0)
				nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1)* nMapWidth + (x + 0)]);
			if(y<nMapHeight-1)
				nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1)* nMapWidth + (x + 0)]);
			if(x>0)
				nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0)* nMapWidth + (x - 1)]);
			if(x<nMapWidth-1)
				nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0)* nMapWidth + (x + 1)]);
			if (y>0 && x>0)
			nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x - 1)]);
			if (y<nMapHeight-1 && x>0)
			nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x - 1)]);
			if (y>0 && x<nMapWidth-1)
			nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 1)]);
			if (y<nMapHeight - 1 && x<nMapWidth-1)
			nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 1)]);
			
		}
	}

	nodeStart = &nodes[(nMapHeight / 2)*nMapWidth + 1];
	nodeEnd = &nodes[(nMapHeight / 2)*nMapWidth + nMapWidth - 2];
	return true;

}

bool PathFinding::Solve_AStar() {
	
	pathNodesPos.clear();
	// Reset Navigation Graph - default all node states
	for (int x = 0; x < nMapWidth; x++)
		for (int y = 0; y < nMapHeight; y++)
		{
			nodes[y*nMapWidth + x].bVisited = false;
			nodes[y*nMapWidth + x].fGlobalGoal = INFINITY;
			nodes[y*nMapWidth + x].fLocalGoal = INFINITY;
			nodes[y*nMapWidth + x].parent = nullptr;	// No parents
		}

	auto distance = [](sNode* a, sNode* b) // For convenience
	{
		return sqrtf((a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y));
	};

	auto heuristic = [distance](sNode* a, sNode* b) // So we can experiment with heuristic
	{
		return distance(a, b);
	};

	// Setup starting conditions
	sNode *nodeCurrent = nodeStart;
	nodeStart->fLocalGoal = 0.0f;
	nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

	//startNode added to not tested list
	
	std::list<sNode*> listNotTestedNodes;
	listNotTestedNodes.push_back(nodeStart);

	while (!listNotTestedNodes.empty()) {
		listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });
		
		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
			listNotTestedNodes.pop_front();

		if (listNotTestedNodes.empty())
			break;

		nodeCurrent = listNotTestedNodes.front();
		nodeCurrent->bVisited = true; 

		for (auto nodeNeighbour : nodeCurrent->vecNeighbours) {
			if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == false)
				listNotTestedNodes.push_back(nodeNeighbour);

			float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

			if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal) {
				nodeNeighbour->parent = nodeCurrent;
				nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;
				nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);

			}
		}
	}

	//XMFLOAT2 nodePos = new XMFLOAT2(nodeEnd->x, nodeEnd->y);
	
	XMFLOAT2 pos;
	pos = XMFLOAT2(currentNode->x, currentNode->y);		//firstly we have to add endNode to position's list
	pathNodesPos.push_back(pos);

	while (true)										//then others by theirs parents to the start point
	{
		if (this->currentNode->parent != nullptr) 
		{
			this->currentNode = this->currentNode->parent;
			pos = XMFLOAT2(currentNode->x, currentNode->y);
			pathNodesPos.push_back(pos);
		}
			
		else {
			std::reverse(pathNodesPos.begin(), pathNodesPos.end());
			return true;
		}
			
	}	
}

std::vector<XMFLOAT2> PathFinding::GetPathNodesPosVector() {
	return pathNodesPos;
}

int PathFinding::howManyNodesToEnter() {
	
	sNode* p = nodeEnd;
	int countNodes=0;
	while (p->parent != nullptr) {
		p = p->parent;
		countNodes++;
	}
	return countNodes;
}

//GETTERY
int PathFinding::getMapheight() {
	return nMapHeight;
}

int PathFinding::getMapWidth() {
	return nMapWidth;
}

void PathFinding::setMapWidth(int value) {
	nMapWidth = value;
}

void PathFinding::setMapHeight(int value) {
	nMapHeight = value;
}