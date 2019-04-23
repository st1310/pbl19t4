#include "NodeList.h"

namespace Library
{
	Node* NodeList::MovedToNode(XMFLOAT3 ObjectPosition, std::vector<Node*>& ListOfNode)
	{
		for (Node* nod : ListOfNode)
		{
			if (nod->IsInsideThisNode(ObjectPosition))
			{
				return nod;
			}
		}
		return nullptr;
	}
}