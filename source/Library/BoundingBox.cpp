#include "BoundingBox.h"

namespace Library
{
	BoundingBox::BoundingBox() :
		isTrigger(false), bottomRectanglePoint(), topRectanglePoint(), originalBottomPos(), originalTopPos()
	{}

	BoundingBox::BoundingBox(XMFLOAT3 bottom, XMFLOAT3 top):
		isTrigger(false), bottomRectanglePoint(), topRectanglePoint(), originalBottomPos(), originalTopPos()
	{
		bottomRectanglePoint = bottom;
		originalBottomPos = bottomRectanglePoint;
		topRectanglePoint = top;
		originalTopPos = topRectanglePoint;

	}

	BoundingBox::~BoundingBox()
	{}

	const XMFLOAT3& BoundingBox::BottomRect() const
	{
		return bottomRectanglePoint;
	}

	const XMFLOAT3& BoundingBox::TopRect() const
	{
		return topRectanglePoint;
	}

	void BoundingBox::SetBottomRect(XMFLOAT3 newBottom)
	{
		bottomRectanglePoint = newBottom;
		originalBottomPos = bottomRectanglePoint;
	}

	void BoundingBox::SetTopRect(XMFLOAT3 newTop)
	{
		topRectanglePoint = newTop;
		originalTopPos = topRectanglePoint;
	}

	void BoundingBox::ChangeTriggerableStatus()
	{
		isTrigger = !isTrigger;
	}

	bool BoundingBox::IsThisTrigger()
	{
		return isTrigger;
	}

	bool BoundingBox::HasDeclaredPoints()
	{
		return (bottomRectanglePoint.x && bottomRectanglePoint.y && bottomRectanglePoint.z
			&& topRectanglePoint.x && topRectanglePoint.y && topRectanglePoint.z);
	}

	//Implies that both have set positions
	bool BoundingBox::Intersects(BoundingBox targetedBox)
	{
		XMFLOAT3 targetBottom = targetedBox.BottomRect();
		XMFLOAT3 targetTop = targetedBox.TopRect();

		if (targetTop.x <= targetBottom.x)
		{
			float helper = targetTop.x;
			targetTop.x = targetBottom.x;
			targetBottom.x = helper;
		}
		if (targetTop.y <= targetBottom.y)
		{
			float helper = targetTop.y;
			targetTop.y = targetBottom.y;
			targetBottom.y = helper;
		}
		if (targetTop.z <= targetBottom.z)
		{
			float helper = targetTop.z;
			targetTop.z = targetBottom.z;
			targetBottom.z = helper;
		}

		return (bottomRectanglePoint.x <= targetTop.x && topRectanglePoint.x >= targetBottom.x)
			&& (bottomRectanglePoint.y <= targetTop.y && topRectanglePoint.y >= targetBottom.y)
			&& (bottomRectanglePoint.z <= targetTop.z && topRectanglePoint.z >= targetBottom.z);
	}

	void BoundingBox::Move(XMVECTOR destination)
	{
		XMFLOAT3 directionFl;
		XMStoreFloat3(&directionFl, destination);


		bottomRectanglePoint = directionFl;
		topRectanglePoint = directionFl;

		bottomRectanglePoint.x = originalBottomPos.x + directionFl.x;
		bottomRectanglePoint.y = originalBottomPos.y + directionFl.y;
		bottomRectanglePoint.z = originalBottomPos.z + directionFl.z;

		topRectanglePoint.x = originalTopPos.x + directionFl.x;
		topRectanglePoint.y = originalTopPos.y + directionFl.y;
		topRectanglePoint.z = originalTopPos.z + directionFl.z;
	}
}