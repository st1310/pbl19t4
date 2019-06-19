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
		void BuildBoundingBox(XMFLOAT3 position, XMFLOAT3 radius);

		void BuildOBB(Mesh* meshes, XMFLOAT4 orientation);
		void BuildOBB(XMFLOAT3 position, XMFLOAT3 radius, XMFLOAT4 orientation);
		void BuildSphere(XMFLOAT3 position, float radius);

		bool IsEmpty();
		void setTriggerReaction(TypesTriggerReactions trg, XMFLOAT3 centerOf, XMFLOAT3 radOfTrig);

		void Transform(CXMMATRIX rotation, XMVECTOR destination);
		void Rotate(XMMATRIX rotation);

		void PushNewBoundingBox(BoundingBox* bbox);
		void PushNewOrientedBoundingBox(BoundingOrientedBox* obbox);
		void PushNewSphere(BoundingSphere* bbSph);

		bool CheckCollision(std::vector<Colliders*>& CollidableObjects);

		bool CheckTriggerCollision(Colliders& TriggerCollider);
		bool CheckTriggerCollisionPaintingPosition(XMFLOAT3 position);

		bool CheckColliderIntersecteByRay(XMVECTOR origin, XMVECTOR direct, float distance);
		bool CheckColliderIntersectsByPlanes(XMVECTOR ray1, XMVECTOR ray2, XMVECTOR camPos, XMVECTOR camDir);

		std::vector<BoundingBox*> GetBoundingBox();

		void removeTrigger(TypesTriggerReactions trg);
		std::vector<TypesTriggerReactions> getTriggeredReactions(XMFLOAT3 checkPos);
		std::vector< std::pair<TypesTriggerReactions, BoundingBox*> > getTriggers();


	private:
		void FindMax(Mesh* meshes, XMFLOAT3* minVec, XMFLOAT3* maxVec);

		std::vector<BoundingOrientedBox*> OrrBoundingBox;
		std::vector<BoundingBox*> BoundingBoxes;
		std::vector<BoundingSphere*> BoundingSpheres;
		std::vector< std::pair<TypesTriggerReactions, BoundingBox*> > TriggerBoxes;
	};
}