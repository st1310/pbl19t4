#include "Node.h"

namespace Library
{
	//A - top left, C - down right
	Node::Node(XMFLOAT3 positionA, XMFLOAT3 positionC)
	{
		mPositionA = positionA;
		mPositionC = positionC;
	}

	Node::~Node()
	{
		mStaticObjects.clear();
		mDynamicObjects.clear();
	}

	void Node::AddStaticCollider(Colliders* staticCollider)
	{
		mStaticObjects.push_back(*staticCollider);
	}

	void Node::AddDynamicCollider(Colliders* dynamicCollider)
	{
		mDynamicObjects.push_back(*dynamicCollider);
	}

	void Node::AddTriggerCollider(Colliders* trigercCollider)
	{
		mTriggers.push_back(*trigercCollider);
	}

	bool Node::IsInsideThisNode(XMFLOAT3 position)
	{
		return (mPositionA.x >= position.x && mPositionC.x <= position.x)
			&& (mPositionA.y <= position.y && mPositionC.y >= position.y);
	}

	//This is used to determine - is movable object inside this node collides with anything within this node
	//So, if there is no movable objects or object doesn't belong here - return false
	bool Node::CheckCollisionInNode(Colliders movingCollider)
	{
		bool collided = false;

		if (mDynamicObjects.empty())
			return false;

		std::vector<Colliders*> vct;
		for (unsigned int i = 0; i < mDynamicObjects.size(); i++)
			vct.push_back(&mDynamicObjects[i]);

		collided = movingCollider.CheckCollision(vct);

		if (collided)
			return true;

		vct.clear();
		for (unsigned int i = 0; i < mStaticObjects.size(); i++)
			vct.push_back(&mStaticObjects[i]);
		collided = movingCollider.CheckCollision(vct);

		return collided;
	}

	//Check, if object from outside is collides with anything in this node.
	//Before this metod should be checked - if object is trying to move in this node or not
	//If yes - only then check collision. In succes - change object node to this.
	bool Node::CheckCollisionWhenEntering(Colliders movingCollider)
	{
		bool collided = false;
		std::vector<Colliders*> vct;

		if (mDynamicObjects.empty() && mStaticObjects.empty())
			return false;

		if (!mDynamicObjects.empty())
		{
			for (unsigned int i = 0; i < mDynamicObjects.size(); i++)
				vct.push_back(&mDynamicObjects[i]);

			collided = movingCollider.CheckCollision(vct);
			vct.clear();
			if (collided)
				return true;
		}

		if (!mStaticObjects.empty())
		{
			for (unsigned int i = 0; i < mStaticObjects.size(); i++)
				vct.push_back(&mStaticObjects[i]);

			collided = movingCollider.CheckCollision(vct);

			vct.clear();
		}

		return collided;
	}
}