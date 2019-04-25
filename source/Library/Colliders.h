#pragma once

#include "Common.h"
#include "Mesh.h"
#include "DrawableGameComponent.h"

namespace Library
{
	class Colliders
	{
	public:
		Colliders();
		Colliders(BoundingBox* colliderBox);
		~Colliders();

		BoundingBox* BuildBoundingBox(Mesh* meshes);

		bool IsEmpty();
		void Move(XMVECTOR destination);
		void Rotate(CXMMATRIX direction);
		void Transform(CXMMATRIX rotation, XMVECTOR destination);
		void PushNewBoundingBox(BoundingBox* bbox);
		bool CheckCollision(std::vector<Colliders*>& CollidableObjects);
		bool CheckTriggerCollision(Colliders& TriggerCollider);
	private:
		std::vector<BoundingBox*> BoundingBoxes;
		std::vector<BoundingBox*> TriggerBoxes;
	};
}