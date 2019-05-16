#include "PathFinding.h"
#include <list>

PathFinding::PathFinding() {

}

PathFinding::~PathFinding() {

}

bool PathFinding::OnUserCreate() {
	
	//grid of nodes
	for (int x = 0; x < nMapWidth; x++) {
		for (int y = 0; y < nMapHeight; y++) {
			nodes[y*nMapWidth + x].x = x;
			nodes[y*nMapHeight + x].y = y;
			nodes[y*nMapHeight + x].bObstacle = false;
			nodes[y*nMapHeight + x].bVisited = false;
			nodes[y*nMapHeight + x].parent = nullptr;
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
	return true;
}

bool PathFinding::OnUserUpdate(float fElapsedTime) {
	
	Solve_AStar();
	//additional things, for visual like for mouseComponent etc.
	return true;
}
