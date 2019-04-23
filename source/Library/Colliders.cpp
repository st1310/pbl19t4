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
		newBox = new BoundingBox(minVec, maxVec);
		//Check how it will create such collider - if needed, will write moving to model position
		return newBox;
	}
	
	bool Colliders::IsEmpty()
	{
		if (BoundingBoxes.empty() && TriggerBoxes.empty())
			return true;
		else return false;
	}

	void Colliders::Move(XMVECTOR destination)
	{
		for (BoundingBox* bbox : BoundingBoxes)
		{
			bbox->Move(destination);
		}
		if (!TriggerBoxes.empty())
		{ 
			for (BoundingBox* tbox : TriggerBoxes)
			{
				tbox->Move(destination);
			}
		}
	}

	void Colliders::Rotate(CXMMATRIX direction)
	{
		XMVECTOR botDir;
		XMVECTOR topDir;
		XMFLOAT3 rewriter;

		for (BoundingBox* bbox : BoundingBoxes)
		{
			botDir = XMLoadFloat3(&bbox->BottomRect());
			botDir = XMVector3TransformNormal(botDir, direction);
			botDir = XMVector3Normalize(botDir);

			XMStoreFloat3(&rewriter, botDir);
			bbox->SetBottomRect(rewriter);

			topDir = XMLoadFloat3(&bbox->TopRect());
			topDir = XMVector3TransformNormal(topDir, direction);
			topDir = XMVector3Normalize(topDir);

			XMStoreFloat3(&rewriter, topDir);
			bbox->SetTopRect(rewriter);
		}

		if (!TriggerBoxes.empty())
		{
			for (BoundingBox* tbox : TriggerBoxes)
			{
				botDir = XMLoadFloat3(&tbox->BottomRect());
				botDir = XMVector3TransformNormal(botDir, direction);
				botDir = XMVector3Normalize(botDir);

				XMStoreFloat3(&rewriter, botDir);
				tbox->SetBottomRect(rewriter);

				topDir = XMLoadFloat3(&tbox->TopRect());
				topDir = XMVector3TransformNormal(topDir, direction);
				topDir = XMVector3Normalize(topDir);

				XMStoreFloat3(&rewriter, topDir);
				tbox->SetTopRect(rewriter);
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
						if(bbox->HasDeclaredPoints() && tbbox->HasDeclaredPoints())
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