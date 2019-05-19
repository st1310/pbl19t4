#pragma once
#include <vector>


class PathFinding {
public:

	PathFinding();
	~PathFinding();

	int getMapWidth();
	int getMapheight();

	bool OnUserCreate();
	bool Solve_AStar();
	int howManyNodesToEnter();

//private:

	struct sNode {
		bool bObstacle = false;
		bool bVisited = false;
		float fGlobalGoal;
		float fLocalGoal;
		int x;
		int y;
		std::vector<sNode*> vecNeighbours;
		sNode* parent;
	};

	sNode * nodes = nullptr;
	int nMapWidth = 10;
	int nMapHeight =10;

	sNode *nodeStart = nullptr;
	sNode *nodeEnd = nullptr;
	sNode *currentNode = nullptr;

protected:

	//bool OnUserCreate();
	//bool Solve_AStar();
	//bool OnUserUpdate(float fElapsedTime);

};