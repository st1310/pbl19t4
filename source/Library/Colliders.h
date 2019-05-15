#pragma once

#include "Common.h"
#include "Mesh.h"

namespace Library
{
	enum TypesTriggerReactions
	{
		POLICE_CATCHING,
		POLICE_ALLIES,
		POLICE_STATION,
		PLAYER_UNIT,
		PAINT,
		PAINTING_POSITION
	};

	class Colliders
	{
	public:
		Colliders();
		Colliders(BoundingBox* colliderBox);
		~Colliders();

		void BuildBoundingBox(Mesh* meshes);

		bool IsEmpty();
		void setTriggerReaction(TypesTriggerReactions trg, XMFLOAT3 centerOf, XMFLOAT3 radOfTrig);

		void Transform(CXMMATRIX rotation, XMVECTOR destination);
		void PushNewBoundingBox(BoundingBox* bbox);
		bool CheckCollision(std::vector<Colliders*>& CollidableObjects);
		bool CheckTriggerCollision(Colliders& TriggerCollider);

		bool CheckColliderIntersecteByRay(XMVECTOR origin, XMVECTOR direct, float distance);

		void removeTrigger(TypesTriggerReactions trg);
		std::vector<TypesTriggerReactions> getTriggeredReactions(XMFLOAT3 checkPos);
		std::vector< std::pair<TypesTriggerReactions, BoundingBox*> > getTriggers();
	private:
		std::vector<BoundingBox*> BoundingBoxes;
		std::vector< std::pair<TypesTriggerReactions, BoundingBox*> > TriggerBoxes;
	};
}