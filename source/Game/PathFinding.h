#pragma once
#include <vector>

class PathFinding {
public:

	PathFinding();
	~PathFinding();

private:

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
	int nMapWidth = 16;
	int nMapHeight = 16;

	sNode *nodeStart = nullptr;
	sNode *nodeEnd = nullptr;

protected:

	bool OnUserCreate();
	bool Solve_AStar();
	bool OnUserUpdate(float fElapsedTime);

};