#include "Colliders.h"

namespace Library
{

	Colliders::Colliders() :
		OrrBoundingBox(), BoundingBoxes(), TriggerBoxes()
	{
	}

	Colliders::Colliders(BoundingBox* colliderBox) :
		OrrBoundingBox(), BoundingBoxes(), TriggerBoxes()
	{
		BoundingBoxes.push_back(colliderBox);
	}

	Colliders::~Colliders()
	{
		BoundingBoxes.clear();
		TriggerBoxes.clear();
	}

	std::vector<BoundingBox*> Colliders::GetBoundingBox()
	{
		return BoundingBoxes;
	}


	void Colliders::PushNewBoundingBox(BoundingBox* bbox)
	{
		BoundingBoxes.push_back(bbox);
	}

	void Colliders::PushNewOrientedBoundingBox(BoundingOrientedBox* obbox)
	{
		OrrBoundingBox.push_back(obbox);
	}


	//Internal method - it is used in 2 others for building (oriented)boundingboxes
	void Colliders::FindMax(Mesh* meshes, XMFLOAT3* minVec, XMFLOAT3* maxVec)
	{

		for (XMFLOAT3 meshPart : meshes->Vertices())
		{
			if (minVec->x > meshPart.x)
				minVec->x = meshPart.x;
			if (minVec->y > meshPart.y)
				minVec->y = meshPart.y;
			if (minVec->z > meshPart.z)
				minVec->z = meshPart.z;

			if (maxVec->x < meshPart.x)
				maxVec->x = meshPart.x;
			if (maxVec->y < meshPart.y)
				maxVec->y = meshPart.y;
			if (maxVec->z < meshPart.z)
				maxVec->z = meshPart.z;
		}
	}
	
	void Colliders::BuildBoundingBox(Mesh* meshes)
	{
		XMFLOAT3 minVec;
		XMFLOAT3 maxVec;

		minVec = meshes->Vertices().at(0);
		maxVec = meshes->Vertices().at(0);

		FindMax(meshes, &minVec, &maxVec);
		
		
		XMFLOAT3 center, rad;
		center.x = (maxVec.x + minVec.x) * 0.5f;
		center.y = (maxVec.y + minVec.y) * 0.5f;
		center.z = (maxVec.z + minVec.z) * 0.5f;

		rad.x = abs(maxVec.x - center.x);
		rad.y = abs(maxVec.y - center.y);
		rad.z = abs(maxVec.z - center.z);
		
		BoundingBox* newBox;
		newBox = new BoundingBox(center, rad);

		PushNewBoundingBox(newBox);
	}

	//If models vertexes are messed, use this
	void Colliders::BuildBoundingBox(XMFLOAT3 position, XMFLOAT3 radius)
	{
		BoundingBox* newBox = new BoundingBox(position, radius);
		PushNewBoundingBox(newBox);
	}
	
	void Colliders::BuildOBB(Mesh* meshes, XMFLOAT4 orientation)
	{
		XMFLOAT3 minVec;
		XMFLOAT3 maxVec;

		minVec = meshes->Vertices().at(0);
		maxVec = meshes->Vertices().at(0);

		FindMax(meshes, &minVec, &maxVec);

		XMFLOAT3 center, rad;
		center.x = (maxVec.x + minVec.x) * 0.5f;
		center.y = (maxVec.y + minVec.y) * 0.5f;
		center.z = (maxVec.z + minVec.z) * 0.5f;

		rad.x = abs(maxVec.x - center.x);
		rad.y = abs(maxVec.y - center.y);
		rad.z = abs(maxVec.z - center.z);

		BoundingOrientedBox* newBox;
		newBox = new BoundingOrientedBox(center, rad, orientation);

		PushNewOrientedBoundingBox(newBox);
	}

	//If models vertexes are messed, use this
	void Colliders::BuildOBB(XMFLOAT3 position, XMFLOAT3 radius, XMFLOAT4 orientation)
	{
		BoundingOrientedBox* newBox = new BoundingOrientedBox(position, radius, orientation);
		PushNewOrientedBoundingBox(newBox);
	}

	bool Colliders::IsEmpty()
	{
		if (BoundingBoxes.empty() && TriggerBoxes.empty() && OrrBoundingBox.empty())
			return true;
		else return false;
	}

	void Colliders::setTriggerReaction(TypesTriggerReactions trg, XMFLOAT3 centerOf, XMFLOAT3 radOfTrig)
	{
		if (TriggerBoxes.empty())
		{
			TriggerBoxes.push_back({ trg, new BoundingBox(centerOf, radOfTrig) });
		}
		else 
		{
			//if (std::find(TriggerBoxes.begin(), TriggerBoxes.end(), trg) == TriggerBoxes.end())
			for (int i = 0; i < TriggerBoxes.size(); i++)
			{
				if (trg == TriggerBoxes[i].first)
					return;
			}
			TriggerBoxes.push_back({ trg, new BoundingBox(centerOf, radOfTrig) });
		}
	}

	void Colliders::Transform(CXMMATRIX rotation, XMVECTOR destination)
	{
		XMMATRIX trMatr = XMMATRIX({1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
		XMFLOAT3 movm;

		for (BoundingBox* bbox : BoundingBoxes)
		{
			XMStoreFloat3(&movm, destination);
			movm.x = movm.x - bbox->Center.x;
			movm.y = movm.y - bbox->Center.y;
			movm.z = movm.z - bbox->Center.z;

			trMatr.r[3] = XMLoadFloat3(&movm);
			trMatr.r[3].m128_f32[3] = 1.0f;
			trMatr = XMMatrixMultiply(rotation, trMatr);

			bbox->Transform(*bbox, trMatr);
		}

		for (BoundingOrientedBox* obbox : OrrBoundingBox)
		{
			XMStoreFloat3(&movm, destination);
			movm.x = movm.x - obbox->Center.x;
			movm.y = movm.y - obbox->Center.y;
			movm.z = movm.z - obbox->Center.z;

			trMatr.r[3] = XMLoadFloat3(&movm);
			trMatr.r[3].m128_f32[3] = 1.0f;
			trMatr = XMMatrixMultiply(rotation, trMatr);

			obbox->Transform(*obbox, trMatr);
		}

		if (!TriggerBoxes.empty())
		{
			for (std::pair<TypesTriggerReactions, BoundingBox*> tbox : TriggerBoxes)
			{
				XMStoreFloat3(&movm, destination);
				movm.x = movm.x - tbox.second->Center.x;
				movm.y = movm.y - tbox.second->Center.y;
				movm.z = movm.z - tbox.second->Center.z;

				trMatr.r[3] = XMLoadFloat3(&movm);
				trMatr.r[3].m128_f32[3] = 1.0f;
				trMatr = XMMatrixMultiply(rotation, trMatr);

				tbox.second->Transform(*tbox.second, trMatr);
			}
		}
	}


	//Check collision of this MOVABLE collider with list of static colliders
	bool Colliders::CheckCollision(std::vector<Colliders*>& CollidableObjects)
	{
		bool colidable = false;
		if ( (BoundingBoxes.empty() && OrrBoundingBox.empty() ) || CollidableObjects.empty())
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
							return true;
					}

					for (BoundingOrientedBox* tbbox : coll->OrrBoundingBox)
					{
						if (bbox->Intersects(*tbbox))
							return true;
					}
				}		
			}
		}
		return false;
	}

	bool Colliders::CheckTriggerCollisionPaintingPosition(XMFLOAT3 position)
	{
		for (int i = 0; i < TriggerBoxes.size(); i++)
		{
			if ((TriggerBoxes[i].second->Intersects(XMLoadFloat3(&position)) != DISJOINT) || (TriggerBoxes[i].first == PAINTING_POSITION))
			{
				return true;
			}
		}
		return false;
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

	bool Colliders::CheckColliderIntersecteByRay(XMVECTOR origin, XMVECTOR direct, float distance)
	{
		for (BoundingBox* bbox : BoundingBoxes)
		{
			if (bbox->Intersects(origin, direct, distance))
				return true;
		}
		if (!OrrBoundingBox.empty())
		{
			for (BoundingOrientedBox* obbox : OrrBoundingBox)
			{
				if (obbox->Intersects(origin, direct, distance))
					return true;
			}
		}
		return false;
	}

	bool Colliders::CheckColliderIntersectsByPlanes(XMVECTOR ray1, XMVECTOR ray2, XMVECTOR camPos, XMVECTOR camDir)
	{
		std::vector<XMVECTOR> planes;
		//right slope
		if (ray1.m128_f32[0] < ray2.m128_f32[0])
		{
			planes.push_back(XMVectorSet(1.0f, 0.f, -ray1.m128_f32[0], 0.f));
			planes.push_back(XMVectorSet(-1.0f, 0.f, ray2.m128_f32[0], 0.f));
		}
		else
		{
			planes.push_back(XMVectorSet(1.0f, 0.f, -ray2.m128_f32[0], 0.f));
			planes.push_back(XMVectorSet(-1.0f, 0.f, ray1.m128_f32[0], 0.f));
		}


		planes[0] = DirectX::Internal::XMPlaneTransform(planes[0], camDir, camPos);
		planes[0] = XMPlaneNormalize(planes[0]);

		//left slope
		planes[1] = DirectX::Internal::XMPlaneTransform(planes[1], camDir, camPos);
		planes[1] = XMPlaneNormalize(planes[1]);

		if (ray1.m128_f32[1] < ray2.m128_f32[1])
		{
			planes.push_back(XMVectorSet(0.0f, 1.f, -ray1.m128_f32[1], 0.f));
			planes.push_back(XMVectorSet(0.0f, -1.f, ray2.m128_f32[1], 0.f));
		}
		else
		{
			planes.push_back(XMVectorSet(0.0f, 1.f, -ray2.m128_f32[1], 0.f));
			planes.push_back(XMVectorSet(0.0f, -1.f, ray1.m128_f32[1], 0.f));
		}

		//upper slope
		planes[2] = DirectX::Internal::XMPlaneTransform(planes[2], camDir, camPos);
		planes[2] = XMPlaneNormalize(planes[2]);

		//lower slope
		planes[3] = DirectX::Internal::XMPlaneTransform(planes[3], camDir, camPos);
		planes[3] = XMPlaneNormalize(planes[3]);

		//near and far slope
		if (camDir.m128_f32[2] > 0)
		{
			planes.push_back(XMVectorSet(0.0f, 0.f, -1.0f, (camPos.m128_f32[2] - 0.5f)));
			planes.push_back(XMVectorSet(0.0f, 0.f, 1.0f, -(camPos.m128_f32[2] + 1000.f)));
		}
		else
		{
			planes.push_back(XMVectorSet(0.0f, 0.f, 1.0f, -(camPos.m128_f32[2] + 0.5f)));
			planes.push_back(XMVectorSet(0.0f, 0.f, -1.0f, (camPos.m128_f32[2] - 1000.f)));
		}

		planes[4] = DirectX::Internal::XMPlaneTransform(planes[4], camDir, camPos);
		planes[4] = XMPlaneNormalize(planes[4]);

		planes[5] = DirectX::Internal::XMPlaneTransform(planes[5], camDir, camPos);
		planes[5] = XMPlaneNormalize(planes[5]);

		for (BoundingBox* bbox : BoundingBoxes)
		{
			if (bbox->ContainedBy(planes[0], planes[1], planes[2], planes[3], planes[4], planes[5]) != DISJOINT)
				return true;
		}
		if (!OrrBoundingBox.empty())
		{
			for (BoundingOrientedBox* obbox : OrrBoundingBox)
			{
				if (obbox->ContainedBy(planes[0], planes[1], planes[2], planes[3], planes[4], planes[5]) != DISJOINT)
					return true;
			}
		}
		return false;
	}

	void Colliders::removeTrigger(TypesTriggerReactions trg)
	{
		for (int i = 0; i < TriggerBoxes.size(); i++)
		{
			if (TriggerBoxes[i].first == trg)
			{
				TriggerBoxes.erase(TriggerBoxes.begin() + i);
				return;
			}
		}
	}

	std::vector<TypesTriggerReactions> Colliders::getTriggeredReactions(XMFLOAT3 checkPos)
	{
		std::vector<TypesTriggerReactions> answer;
		for (int i = 0; i < TriggerBoxes.size(); i++)
		{
			if (TriggerBoxes[i].second->Contains(XMLoadFloat3(&checkPos)))
				answer.push_back(TriggerBoxes[i].first);
		}
		return answer;
	}
	
	std::vector< std::pair<TypesTriggerReactions, BoundingBox*> > Colliders::getTriggers()
	{
		return TriggerBoxes;
	}
}