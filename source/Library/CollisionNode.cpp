#include "CollisionNode.h"

namespace Library
{
	//A - top left, C - down right
	CollisionNode::CollisionNode(XMFLOAT3 positionA, XMFLOAT3 positionC)
		: mStaticObjects(), mDynamicObjects(), mTriggers(), mChilds(), mParent(nullptr)
	{
		mPositionA = positionA;
		mPositionC = positionC;
	}

	CollisionNode::~CollisionNode()
	{
		mStaticObjects.clear();
		mDynamicObjects.clear();
		mTriggers.clear();
		mChilds.clear();
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

	void CollisionNode::AddTriggerCollider(Colliders* trigercCollider)
	{
		mTriggers.push_back(trigercCollider);
	}

	void CollisionNode::SetParent(CollisionNode* newParent)
	{
		mParent = newParent;
	}

	void CollisionNode::AddNewChild(CollisionNode* newChild)
	{
		mChilds.push_back(newChild);
	}

	void CollisionNode::SetNewChildList(std::vector<CollisionNode*> newChilds)
	{
		mChilds = newChilds;
	}

	CollisionNode* CollisionNode::GetParent()
	{
		return mParent;
	}

	std::vector<CollisionNode*> CollisionNode::GetChilds()
	{
		return mChilds;
	}

	bool CollisionNode::IsThisNodeEmpty()
	{
		return (mStaticObjects.empty() && mDynamicObjects.empty() && mTriggers.empty());
	}

	bool CollisionNode::IsInsideThisNode(XMFLOAT3 position)
	{
		return (mPositionA.x >= position.x && mPositionC.x <= position.x)
			&& (mPositionA.y <= position.y && mPositionC.y >= position.y);
	}

	//This is used to determine - is movable object inside this node collides with anything within this node
	//So, if there is no movable objects or object doesn't belong here - return false
	bool CollisionNode::CheckCollisionInNode(Colliders* movingCollider)
	{
		bool collided = false;

		if (mDynamicObjects.empty())
			return false;

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

		return collided;
	}

	//Check, if object from outside is collides with anything in this node.
	//Before this metod should be checked - if object is trying to move in this node or not
	//If yes - only then check collision. In succes - change object node to this.
	bool CollisionNode::CheckCollisionWhenEntering(Colliders* movingCollider)
	{
		bool collided = false;
		std::vector<Colliders*> vct;

		if (mDynamicObjects.empty() && mStaticObjects.empty())
			return false;

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

	bool CollisionNode::IsCatchedByFrustum(BoundingFrustum* bFrst)
	{
		BoundingBox bbox;

		bbox.CreateFromPoints(bbox, XMLoadFloat3(&mPositionA), XMLoadFloat3(&mPositionC));

		//If Node is CONTAINED or INTERSECTS with Frustrum - return true;
		if (bFrst->Contains(bbox) != DISJOINT)
			return true;
		else return false;
	}
}