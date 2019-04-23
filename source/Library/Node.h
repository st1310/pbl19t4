#pragma once

#include "Colliders.h"


namespace Library
{
	class Node
	{
	public:
		Node(XMFLOAT3 positionA, XMFLOAT3 positionC);
		~Node();
		
		void AddStaticCollider(Colliders* staticCollider);
		void AddDynamicCollider(Colliders* dynamicCollider);
		void AddTriggerCollider(Colliders* trigercCollider);

		bool IsInsideThisNode(XMFLOAT3 position);

		bool CheckCollisionInNode(Colliders movingCollider);
		bool CheckCollisionWhenEntering(Colliders movingCollider);

	private:
		XMFLOAT3 mPositionA;
		XMFLOAT3 mPositionC;
		std::vector<Colliders> mStaticObjects;
		std::vector<Colliders> mDynamicObjects;
		std::vector<Colliders> mTriggers;
	};
}