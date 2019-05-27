
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
			nodes[y*nMapWidth + x].x = -75 + y;
			nodes[y*nMapWidth + x].y = 5 + x;
			nodes[y*nMapWidth + x].bObstacle = false;
			nodes[y*nMapWidth + x].bVisited = false;
			nodes[y*nMapWidth + x].parent = nullptr;
			nodes[y*nMapWidth + x].bbox = new BoundingBox(XMFLOAT3(nodes[y*nMapWidth + x].x, .0f, nodes[y*nMapWidth + x].y), XMFLOAT3(1.f, 1.f, 1.f));
			collider->AddBoundingBox(nodes[y*nMapWidth + x].bbox);
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
			if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
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
	
	while (true) {
		if (this->currentNode->parent != nullptr) {
			this->currentNode = this->currentNode->parent;
			XMFLOAT2 pos = XMFLOAT2(currentNode->x, currentNode->y);
			pathNodesPos.push_back(pos);
		}
			
		else
			return true;
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