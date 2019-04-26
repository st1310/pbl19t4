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


	//Use result to check if drawable component is in this nodes
	std::vector<CollisionNode*> NodeList::CheckNodesInsideCamera(BoundingFrustum* bFrst, std::vector<CollisionNode*> listOfNode)
	{
		std::vector<CollisionNode*> helperList;
		if ( (!listOfNode.empty()) && (bFrst != nullptr))
		for (CollisionNode* clNd : listOfNode)
		{
			if (clNd->IsCatchedByFrustum(bFrst))
			{
				if (!clNd->GetChilds().empty())
				{
					std::vector<CollisionNode*> minorHelperList;
					minorHelperList = CheckNodesInsideCamera(bFrst, clNd->GetChilds());
					helperList.insert(helperList.end(), minorHelperList.begin(), minorHelperList.end());

					minorHelperList.clear();
				}
				else helperList.push_back(clNd);
			}
		}

		return helperList;
	}

	bool NodeList::IsNodeInsideList(CollisionNode* checkedNode, std::vector<CollisionNode*> listOfNode)
	{
		if (std::find(listOfNode.begin(), listOfNode.end(), checkedNode) != listOfNode.end())
			return true;
		else return false;
	}
}
