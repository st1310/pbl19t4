#pragma once

#include "CollisionNode.h"
#include "DrawableGameComponent.h"
//include some sort of entity.h which contains model with collision?

namespace Library
{
	class NodeList
	{
	public:
		static CollisionNode* MovedToNode(XMFLOAT3 ObjectPosition, std::vector<CollisionNode*>& ListOfNode);
		static std::vector<CollisionNode*> CheckNodesInsideCamera(BoundingFrustum* bFrst, std::vector<CollisionNode*> listOfNode);
		static bool IsNodeInsideList(CollisionNode* checkedNode, std::vector<CollisionNode*> listOfNode);
	private:
		NodeList();
	};
}