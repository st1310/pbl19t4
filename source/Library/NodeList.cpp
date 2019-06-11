#include "NodeList.h"

namespace Library
{
	//Method to check, if moving is possible considering collision
	//If true - move position of object containing this Collider to targetPosition
	bool NodeList::TryToMoveInNode(Colliders* coll, XMVECTOR originalPosition, CXMMATRIX rotation, XMVECTOR targetPosition, CollisionNode* orgPosNode)
	{
		if (coll->IsEmpty())
			return true;

		if (abs(targetPosition.m128_f32[1] - originalPosition.m128_f32[1]) > 1.f)
			return false;

		coll->Transform(rotation, targetPosition);

		XMFLOAT3 checkPos;
		XMStoreFloat3(&checkPos, targetPosition);

		if (orgPosNode->IsInsideThisNode(checkPos))
		{
			if (!orgPosNode->CheckCollisionInNode(coll))
				return true;
			else
			{
				if (IsMatrixZero(rotation))
					coll->Transform(rotation, originalPosition);
				else coll->Transform(InverseRotation(rotation), originalPosition);
				return false;
			}
		}
		else
		{
			CollisionNode* newColl = NodeList::MovedToNode(checkPos, orgPosNode);
			if (newColl == nullptr)
			{
				orgPosNode->RemoveDynamicCollider(coll);
				orgPosNode = nullptr;
				return true;
			}
			
			if (!newColl->CheckCollisionWhenEntering(coll))
			{
				orgPosNode->RemoveDynamicCollider(coll);
				newColl->AddDynamicCollider(coll);					
				orgPosNode = newColl;
				return true;
			}
			else return false;
		}
	}

	CollisionNode* NodeList::MovedToChildNode(XMFLOAT3 ObjectPosition, CollisionNode* startinglNode)
	{
		if (startinglNode->GetChild().empty())
			return startinglNode;
		
		for (CollisionNode* collNode : startinglNode->GetChild())
		{
			if (collNode->IsInsideThisNode(ObjectPosition))
			{
				if (collNode->GetChild().empty()) return collNode;
				else return MovedToChildNode(ObjectPosition, collNode);
			}
		}
	}


	CollisionNode* NodeList::MovedToNode(XMFLOAT3 ObjectPosition, CollisionNode* originalNode)
	{
		CollisionNode* parNode;
		if (originalNode->GetParent() != nullptr)
			parNode = originalNode->GetParent();
		else parNode = originalNode;

		if (parNode->IsInsideThisNode(ObjectPosition))
		{
			if (parNode->GetChild().empty())
			{
				return parNode;
			}
			else for (CollisionNode* newNd : parNode->GetChild())
			{
				if (newNd->IsInsideThisNode(ObjectPosition))
				{
					if (newNd->GetChild().empty()) return newNd;
					else return MovedToChildNode(ObjectPosition, newNd);
				}	
			}

			//If you managed to sneak to zone in parent node which isn't assigned to childs;
			return parNode;
		}
		else
		{
			if (parNode->GetParent() != nullptr)
			{
				return MovedToNode(ObjectPosition, parNode);
			}
			else nullptr;
		}
	}

	//when object comes from NULLPTR
	CollisionNode* NodeList::MovedToNode(XMFLOAT3 ObjectPosition, std::vector<CollisionNode*> nodeList)
	{
		CollisionNode* asgNod;
		for (CollisionNode* nod : nodeList)
		{
			if (nod->IsInsideThisNode(ObjectPosition))
			{
				if (nod->GetChild().empty()) return nod;
				else return MovedToChildNode(ObjectPosition, nod);
			}
		}

		return nullptr;
		
	}


	//Use result to check if drawable component is in this nodes
	std::vector<CollisionNode*> NodeList::CheckNodesInsideCamera(std::vector<XMVECTOR> planes, std::vector<CollisionNode*> listOfNode)
	{
		std::vector<CollisionNode*> helperList;
		if ( (!listOfNode.empty()))
		for (CollisionNode* clNd : listOfNode)
		{
			if (clNd->CustomPlaneIntersection(planes))
			{
				if (!clNd->GetChild().empty())
				{
					std::vector<CollisionNode*> minorHelperList;
					minorHelperList = CheckNodesInsideCamera(planes, clNd->GetChild());
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

	bool NodeList::IsMatrixZero(XMMATRIX checkedMatr)
	{
		if ((checkedMatr.r[0].m128_f32[0] == 1.f && checkedMatr.r[0].m128_f32[1] == 0.f && checkedMatr.r[0].m128_f32[2] == 0.f && checkedMatr.r[0].m128_f32[3] == 0.f)
			&& (checkedMatr.r[1].m128_f32[0] == 0.f && checkedMatr.r[1].m128_f32[1] == 1.f && checkedMatr.r[1].m128_f32[2] == 0.f && checkedMatr.r[1].m128_f32[3] == 0.f)
			&& (checkedMatr.r[2].m128_f32[0] == 0.f && checkedMatr.r[2].m128_f32[1] == 0.f && checkedMatr.r[2].m128_f32[2] == 1.f && checkedMatr.r[2].m128_f32[3] == 0.f)
			&& (checkedMatr.r[3].m128_f32[0] == 0.f && checkedMatr.r[3].m128_f32[1] == 0.f && checkedMatr.r[3].m128_f32[2] == 0.f && checkedMatr.r[3].m128_f32[3] == 1.f))
			return true;
		else return false;
	}


	//Since we rotate only by Y - we need invert in matrix only 4 values.
	XMMATRIX NodeList::InverseRotation(XMMATRIX checkedMatr)
	{
		return XMMATRIX({ -checkedMatr.r[0].m128_f32[0],  checkedMatr.r[0].m128_f32[1], -checkedMatr.r[0].m128_f32[2], checkedMatr.r[0].m128_f32[3],
			checkedMatr.r[1].m128_f32[0],  checkedMatr.r[1].m128_f32[1], checkedMatr.r[1].m128_f32[2], checkedMatr.r[1].m128_f32[3],
			-checkedMatr.r[2].m128_f32[0],  checkedMatr.r[2].m128_f32[1], -checkedMatr.r[2].m128_f32[2], checkedMatr.r[2].m128_f32[3],
			checkedMatr.r[3].m128_f32[0],  checkedMatr.r[3].m128_f32[1], checkedMatr.r[3].m128_f32[2], checkedMatr.r[3].m128_f32[3] });
	}
}
