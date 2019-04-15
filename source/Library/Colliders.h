#pragma once

#include "BoundingBox.h"
#include "Mesh.h"
#include "DrawableGameComponent.h"

namespace Library
{
	class Colliders
	{
	public:
		Colliders();
		Colliders(BoundingBox colliderBox);
		~Colliders();

		BoundingBox BuildBoundingBox(Mesh* meshes);

		bool IsEmpty();
		void Move(XMVECTOR destination);
		void Rotate(CXMMATRIX direction);
		void PushNewBoundingBox(BoundingBox bbox);
		bool CheckCollision(std::vector<Colliders*>& CollidableObjects);
		bool CheckTriggerCollision(Colliders& TriggerCollider);
		void DebuggingMode();
	private:
		std::vector<BoundingBox> BoundingBoxes;
		std::vector<BoundingBox> TriggerBoxes;
	};
}