#pragma once

#include "CollisionNode.h"
#include "DrawableGameComponent.h"

namespace Library
{
	class NodeList
	{
	public:
		static bool TryToMoveInNode(Colliders* coll, XMVECTOR originalPosition, CXMMATRIX rotation, XMVECTOR targetPosition, CollisionNode* orgPosNode);
		static CollisionNode* MovedToChildNode(XMFLOAT3 ObjectPosition, CollisionNode* startinglNode);
		static CollisionNode* MovedToNode(XMFLOAT3 ObjectPosition, CollisionNode* originalNode);
		static CollisionNode* MovedToNode(XMFLOAT3 ObjectPosition, std::vector<CollisionNode*> nodeList);
		static std::vector<CollisionNode*> CheckNodesInsideCamera(std::vector<XMVECTOR> planes, std::vector<CollisionNode*> listOfNode);
		static bool IsNodeInsideList(CollisionNode* checkedNode, std::vector<CollisionNode*> listOfNode);

		static bool IsMatrixZero(XMMATRIX checkedMatr);
		static XMMATRIX InverseRotation(XMMATRIX checkedMatr);
	private:
		NodeList();
	};
}