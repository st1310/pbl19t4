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
	private:
		NodeList();
	};
}