#pragma once
#include <vector>
#include <DirectXMath.h>
#include "Colliders.h"
#include "Common.h"

using namespace Library;

class PathFinding {
public:

	PathFinding();
	~PathFinding();

	int getMapWidth();
	int getMapheight();

	void setMapWidth(int value);
	void setMapHeight(int value);

	bool OnUserCreate();
	bool Solve_AStar();
	int howManyNodesToEnter();

//private:

	struct sNode {
		bool bObstacle = false;
		bool bVisited = false;
		bool bMouseCliked = false;
		float fGlobalGoal;
		float fLocalGoal;
		int x;
		int y;
		std::vector<sNode*> vecNeighbours;
		sNode* parent;
		BoundingBox* bbox;
	};

	Colliders* collider;

	sNode * nodes = nullptr;
	int nMapWidth = 10;
	int nMapHeight =10;

	sNode *nodeStart = nullptr;
	sNode *nodeEnd = nullptr;
	sNode *currentNode = nullptr;

	std::vector<XMFLOAT2> pathNodesPos;

	

protected:

	//bool OnUserCreate();
	//bool Solve_AStar();
	//bool OnUserUpdate(float fElapsedTime);

};