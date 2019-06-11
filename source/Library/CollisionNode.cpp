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

		mStaticObjects = std::vector<Colliders*>();
		mDynamicObjects = std::vector<Colliders*>();
		mTriggers = std::vector<Colliders*>();
		mChildList = std::vector<CollisionNode*>();
	}

	CollisionNode::~CollisionNode()
	{
		mStaticObjects.clear();
		mDynamicObjects.clear();
		mTriggers.clear();
		mChildList.clear();
		mParent = nullptr;
	}

	XMFLOAT3 CollisionNode::GetPosA()
	{
		return mPositionA;
	}

	XMFLOAT3 CollisionNode::GetPosC()
	{
		return mPositionC;
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

	bool CollisionNode::IsStaticOnTheWay(Colliders* pathCollider)
	{
		if (mStaticObjects.empty())
			return false;

		std::vector<Colliders*> vct;
		vct.push_back(pathCollider);

		for (unsigned int i = 0; i < mStaticObjects.size(); i++)
			vct.push_back(mStaticObjects[i]);

		return pathCollider->CheckCollision(vct);
	}

	//This is used to determine - is movable object inside this node collides with anything within this node
	//So, if there is no movable objects or object doesn't belong here - return false
	bool CollisionNode::CheckCollisionInNode(Colliders* movingCollider)
	{
		if (mDynamicObjects.empty() && mStaticObjects.empty())
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

	bool CollisionNode::CustomPlaneIntersection(std::vector<XMVECTOR> planes)
	{
		bool intersection = false;

		BoundingBox bbox;
		bbox.CreateFromPoints(bbox, XMLoadFloat3(&mPositionA), XMLoadFloat3(&mPositionC));

		if (bbox.ContainedBy(planes[0], planes[1], planes[2], planes[3], planes[4], planes[5]) != DISJOINT)
			return true;

		return false;
	}

	std::vector<TypesTriggerReactions> CollisionNode::trippedTriggers(XMFLOAT3 objectPos)
	{
		std::vector<TypesTriggerReactions> answer;


		if (this->GetParent() != nullptr)
		{
			return this->GetParent()->trippedTriggers(objectPos);
		}
		else for (Colliders* coll : mTriggers)
		{
			if (mTriggers.empty())
				return answer;

			std::vector<TypesTriggerReactions> helper;
			if (!coll->getTriggers().empty())
			{
				helper = coll->getTriggeredReactions(objectPos);
				if (!helper.empty())
				{
					for (TypesTriggerReactions react : helper)
					{
						answer.push_back(react);
					}
				}
			}
		}

		return answer;
	}
}