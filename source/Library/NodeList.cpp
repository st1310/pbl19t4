#include "NodeList.h"

namespace Library
{
	CollisionNode* NodeList::MovedToNode(XMFLOAT3 ObjectPosition, std::vector<CollisionNode*>& ListOfNode)
	{
		for (CollisionNode* nod : ListOfNode)
		{
			if (nod->IsInsideThisNode(ObjectPosition))
			{
				return nod;
			}
		}
		return nullptr;
	}
}