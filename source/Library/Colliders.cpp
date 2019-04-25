#include "Colliders.h"

namespace Library
{

	Colliders::Colliders() :
		BoundingBoxes(), TriggerBoxes()
	{
	}

	Colliders::Colliders(BoundingBox* colliderBox) :
		BoundingBoxes(), TriggerBoxes()
	{
		BoundingBoxes.push_back(colliderBox);
	}

	Colliders::~Colliders()
	{
		BoundingBoxes.clear();
		TriggerBoxes.clear();
	}

	void Colliders::PushNewBoundingBox(BoundingBox* bbox)
	{
		BoundingBoxes.push_back(bbox);
	}

	
	BoundingBox* Colliders::BuildBoundingBox(Mesh* meshes)
	{
		XMFLOAT3 minVec;
		XMFLOAT3 maxVec;

		minVec = meshes->Vertices().at(0);
		maxVec = meshes->Vertices().at(0);

		for (XMFLOAT3 meshPart : meshes->Vertices())
		{
			if (minVec.x > meshPart.x)
				minVec.x = meshPart.x;
			if (minVec.y > meshPart.y)
				minVec.y = meshPart.y;
			if (minVec.z > meshPart.z)
				minVec.z = meshPart.z;

			if (maxVec.x < meshPart.x)
				maxVec.x = meshPart.x;
			if (maxVec.y < meshPart.y)
				maxVec.y = meshPart.y;
			if (maxVec.z < meshPart.z)
				maxVec.z = meshPart.z;
		}
		BoundingBox* newBox;
		XMFLOAT3 center, rad;
		center.x = (maxVec.x + minVec.x) / 2;
		center.y = (maxVec.y + minVec.y) / 2;
		center.z = (maxVec.z + minVec.z) / 2;

		rad.x = abs(maxVec.x - center.x);
		rad.y = abs(maxVec.y - center.y);
		rad.z = abs(maxVec.z - center.z);
		
		newBox = new BoundingBox(center, rad);
		//Check how it will create such collider - if needed, will write moving to model position
		return newBox;
	}
	
	bool Colliders::IsEmpty()
	{
		if (BoundingBoxes.empty() && TriggerBoxes.empty())
			return true;
		else return false;
	}

	void Colliders::Transform(CXMMATRIX rotation, XMVECTOR destination)
	{
		XMVECTOR rotDir;

		for (BoundingBox* bbox : BoundingBoxes)
		{
			rotDir = XMLoadFloat3(&bbox->Center);
			rotDir = XMVector3TransformNormal(rotDir, rotation);
			rotDir = XMVector3Normalize(rotDir);

			FXMVECTOR xmv = rotDir;

			bbox->Transform(*bbox, 1.0f, rotDir, destination);
		}

		if (!TriggerBoxes.empty())
		{
			for (BoundingBox* tbox : TriggerBoxes)
			{
				rotDir = XMLoadFloat3(&tbox->Center);
				rotDir = XMVector3TransformNormal(rotDir, rotation);
				rotDir = XMVector3Normalize(rotDir);

				tbox->Transform(*tbox, 1.0f, rotDir, destination);
			}
		}
	}

	bool Colliders::CheckCollision(std::vector<Colliders*>& CollidableObjects)
	{
		bool colidable = false;
		if (BoundingBoxes.empty() || CollidableObjects.empty())
			return false;

		for (Colliders* coll : CollidableObjects)
		{
			if (this != coll)
			{
				for (BoundingBox* bbox : BoundingBoxes)
				{
					for (BoundingBox* tbbox : coll->BoundingBoxes)
					{
							if (bbox->Intersects(*tbbox))
								colidable = true;
					}
				}
			}
		}

		return colidable;
	}

	//All trigers must be given to TriggerBoxes during OnTriggerEnter and taken during OnTriggerExit 
	bool Colliders::CheckTriggerCollision(Colliders& TriggerCollider)
	{
		bool colided = false;

		if (BoundingBoxes.empty())
			return false;

		for (BoundingBox* bbox : BoundingBoxes)
		{
			for (BoundingBox* trigger : TriggerCollider.BoundingBoxes)
				if (bbox->Intersects(*trigger))
					colided = true;
		}

		return colided;
	}
}