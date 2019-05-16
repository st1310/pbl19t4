#include "CollisionNode.h"

namespace Library
{
	//A - upper left, C - down right
	//So ensure that A have max X and min Y and Z
	//Accordingly - C has min X and max Y and Z
	CollisionNode::CollisionNode(XMFLOAT3 positionA, XMFLOAT3 positionC)
		: mStaticObjects(), mDynamicObjects(), mTriggers(), mChildList(), mParent(nullptr)
	{
		mPositionA = positionA;
		mPositionC = positionC;
	}

	CollisionNode::~CollisionNode()
	{
		mStaticObjects.clear();
		mDynamicObjects.clear();
		mTriggers.clear();
		mChildList.clear();
		mParent = nullptr;
	}

	void CollisionNode::AddStaticCollider(Colliders* staticCollider)
	{
		mStaticObjects.push_back(staticCollider);
	}

	void CollisionNode::AddDynamicCollider(Colliders* dynamicCollider)
	{
		mDynamicObjects.push_back(dynamicCollider);
	}

	void CollisionNode::RemoveDynamicCollider(Colliders* dynamicCollider)
	{
		mDynamicObjects.erase(remove(mDynamicObjects.begin(), mDynamicObjects.end(), dynamicCollider), mDynamicObjects.end());
	}

	void CollisionNode::AddTriggerCollider(Colliders* trigercCollider)
	{
		mTriggers.push_back(trigercCollider);
	}

	void CollisionNode::RemoveTriggerCollider(Colliders* trigercCollider)
	{
		mTriggers.erase(remove(mTriggers.begin(), mTriggers.end(), trigercCollider), mTriggers.end());
	}

	void CollisionNode::SetParent(CollisionNode* newParent)
	{
		mParent = newParent;
	}

	void CollisionNode::AddNewChild(CollisionNode* newChild)
	{
		mChildList.push_back(newChild);
	}

	void CollisionNode::SetNewChildList(std::vector<CollisionNode*> newChilds)
	{
		mChildList = newChilds;
	}

	CollisionNode* CollisionNode::GetParent()
	{
		return mParent;
	}

	std::vector<CollisionNode*> CollisionNode::GetChild()
	{
		return mChildList;
	}

	bool CollisionNode::IsThisNodeEmpty()
	{
		return (mStaticObjects.empty() && mDynamicObjects.empty() && mTriggers.empty());
	}

	bool CollisionNode::IsInsideThisNode(XMFLOAT3 position)
	{
		return (mPositionA.x >= position.x && mPositionC.x <= position.x)
			&& (mPositionA.y <= position.y && mPositionC.y >= position.y)
			&& (mPositionA.z <= position.z && mPositionC.z >= position.z);
	}

	//This is used to determine - is movable object inside this node collides with anything within this node
	//So, if there is no movable objects or object doesn't belong here - return false
	bool CollisionNode::CheckCollisionInNode(Colliders* movingCollider)
	{
		if (mDynamicObjects.empty())
			return false;

		bool collided = false;
		std::vector<Colliders*> vct;

		for (unsigned int i = 0; i < mDynamicObjects.size(); i++)
			vct.push_back(mDynamicObjects[i]);

		collided = movingCollider->CheckCollision(vct);

		if (collided)
			return true;

		vct.clear();
		for (unsigned int i = 0; i < mStaticObjects.size(); i++)
			vct.push_back(mStaticObjects[i]);
		collided = movingCollider->CheckCollision(vct);

		vct.clear();

		return collided;
	}

	//Check, if object from outside is collides with anything in this node.
	//Before this metod should be checked - if object is trying to move in this node or not
	//If yes - only then check collision. In succes - change object node to this.
	bool CollisionNode::CheckCollisionWhenEntering(Colliders* movingCollider)
	{
		if (mDynamicObjects.empty() && mStaticObjects.empty())
			return false;

		bool collided = false;
		std::vector<Colliders*> vct;

		if (!mDynamicObjects.empty())
		{
			for (unsigned int i = 0; i < mDynamicObjects.size(); i++)
				vct.push_back(mDynamicObjects[i]);

			collided = movingCollider->CheckCollision(vct);
			vct.clear();
			if (collided)
				return true;
		}

		if (!mStaticObjects.empty())
		{
			for (unsigned int i = 0; i < mStaticObjects.size(); i++)
				vct.push_back(mStaticObjects[i]);

			collided = movingCollider->CheckCollision(vct);

			vct.clear();
		}

		return collided;
	}

	ContainmentType CollisionNode::ContainedRightHanded(BoundingFrustum* bFrst, BoundingBox bbox)
	{
		XMVECTOR vOrigin = XMLoadFloat3(&bFrst->Origin);
		XMVECTOR vOrientation = XMLoadFloat4(&bFrst->Orientation);

		// Create 6 planes
		XMVECTOR NearPlane = XMVectorSet(0.0f, 0.0f, 1.0f, -bFrst->Near);
		NearPlane = DirectX::Internal::XMPlaneTransform(NearPlane, vOrientation, vOrigin);
		NearPlane = XMPlaneNormalize(NearPlane);

		XMVECTOR FarPlane = XMVectorSet(0.0f, 0.0f, -1.0f, bFrst->Far);
		FarPlane = DirectX::Internal::XMPlaneTransform(FarPlane, vOrientation, vOrigin);
		FarPlane = XMPlaneNormalize(FarPlane);

		XMVECTOR RightPlane = XMVectorSet(1.0f, 0.0f, -(bFrst->RightSlope), 0.0f);
		RightPlane = DirectX::Internal::XMPlaneTransform(RightPlane, vOrientation, vOrigin);
		RightPlane = XMPlaneNormalize(RightPlane);

		XMVECTOR LeftPlane = XMVectorSet(-1.0f, 0.0f, bFrst->LeftSlope, 0.0f);
		LeftPlane = DirectX::Internal::XMPlaneTransform(LeftPlane, vOrientation, vOrigin);
		LeftPlane = XMPlaneNormalize(LeftPlane);

		XMVECTOR TopPlane = XMVectorSet(0.0f, 1.0f, -bFrst->TopSlope, 0.0f);
		TopPlane = DirectX::Internal::XMPlaneTransform(TopPlane, vOrientation, vOrigin);
		TopPlane = XMPlaneNormalize(TopPlane);

		XMVECTOR BottomPlane = XMVectorSet(0.0f, -1.0f, bFrst->BottomSlope, 0.0f);
		BottomPlane = DirectX::Internal::XMPlaneTransform(BottomPlane, vOrientation, vOrigin);
		BottomPlane = XMPlaneNormalize(BottomPlane);

		return bbox.ContainedBy(NearPlane, FarPlane, RightPlane, LeftPlane, TopPlane, BottomPlane);
	}

	bool CollisionNode::IsCatchedByFrustum(BoundingFrustum* bFrst)
	{
		BoundingBox bbox;
		ContainmentType TypeOfCatch;
		bbox.CreateFromPoints(bbox, XMLoadFloat3(&mPositionA), XMLoadFloat3(&mPositionC));

		if (bFrst->Orientation.z >= 0)
			TypeOfCatch = bFrst->Contains(bbox);
		else TypeOfCatch = ContainedRightHanded(bFrst, bbox);

		//If Node is CONTAINED or INTERSECTS with Frustrum - return true;
		if (TypeOfCatch != DISJOINT)
			return true;
		else return false;
	}

	std::vector<TypesTriggerReactions> CollisionNode::trippedTriggers(XMFLOAT3 objectPos)
	{
		std::vector<TypesTriggerReactions> answer;

		if (mTriggers.empty())
			return answer;

		

		for (Colliders* coll : mTriggers)
		{
			std::vector<TypesTriggerReactions> helper;
			if (!coll->getTriggers().empty())
			{
				helper = coll->getTriggeredReactions(objectPos);
				if (!helper.empty())
				{
					for (TypesTriggerReactions react : helper)
					{
						if (std::find(answer.begin(), answer.end(), react) == answer.end())
							answer.push_back(react);
					}
				}
			}
		}

		return answer;
	}
}