#pragma once

#include "Colliders.h"
#include "Common.h"

namespace Library
{
	class CollisionNode
	{
	public:
		CollisionNode(XMFLOAT3 positionA, XMFLOAT3 positionC);
		~CollisionNode();

		void AddStaticCollider(Colliders* staticCollider);
		void AddDynamicCollider(Colliders* dynamicCollider);
		void RemoveDynamicCollider(Colliders* dynamicCollider);
		void AddTriggerCollider(Colliders* trigercCollider);
		void RemoveTriggerCollider(Colliders* trigercCollider);

		void SetParent(CollisionNode* newParent);
		void AddNewChild(CollisionNode* newChild);
		void SetNewChildList(std::vector<CollisionNode*> newChilds);

		CollisionNode* GetParent();
		std::vector<CollisionNode*> GetChild();

		bool IsThisNodeEmpty();
		bool IsInsideThisNode(XMFLOAT3 position);

		bool CheckCollisionInNode(Colliders* movingCollider);
		bool CheckCollisionWhenEntering(Colliders* movingCollider);

		ContainmentType ContainedRightHanded(BoundingFrustum* bFrst, BoundingBox bbox);
		bool IsCatchedByFrustum(BoundingFrustum* bFrst);
	private:
		CollisionNode* mParent;
		std::vector<CollisionNode*> mChildList;
		XMFLOAT3 mPositionA;
		XMFLOAT3 mPositionC;
		std::vector<Colliders*> mStaticObjects;
		std::vector<Colliders*> mDynamicObjects;
		std::vector<Colliders*> mTriggers;
	};
}