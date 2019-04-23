#pragma once

#include "Node.h"
#include "DrawableGameComponent.h"
//include some sort of entity.h which contains model with collision?

namespace Library
{
	class NodeList
	{
	public:
		static Node* MovedToNode(XMFLOAT3 ObjectPosition, std::vector<Node*>& ListOfNode);
	private:
		NodeList();
	};
}