#include "BoundingBox.h"

namespace Library
{
	BoundingBox::BoundingBox() :
		isTrigger(false), bottomRectanglePoint(nullptr), topRectanglePoint(nullptr)
	{}

	BoundingBox::BoundingBox(XMFLOAT3 bottom, XMFLOAT3 top):
		isTrigger(false), bottomRectanglePoint(nullptr), topRectanglePoint(nullptr)
	{
		bottomRectanglePoint = bottom;
		topRectanglePoint = top;
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
	}

	void BoundingBox::SetTopRect(XMFLOAT3 newTop)
	{
		topRectanglePoint = newTop;
	}

	void BoundingBox::ChangeTriggerableStatus()
	{
		isTrigger = !isTrigger;
	}

	bool BoundingBox::IsThisTrigger()
	{
		return isTrigger;
	}

	//Implies that both have set positions
	bool BoundingBox::Intersects(BoundingBox targetedBox)
	{
		XMFLOAT3 targetBottom = targetedBox.BottomRect();
		XMFLOAT3 targetTop = targetedBox.TopRect();

		return (bottomRectanglePoint.x <= targetTop.x && topRectanglePoint.x >= targetBottom.x)
			&& (bottomRectanglePoint.y <= targetTop.y && topRectanglePoint.y >= targetBottom.y)
			&& (bottomRectanglePoint.z <= targetTop.x && topRectanglePoint.x >= targetBottom.z);
	}

	void BoundingBox::Move(XMVECTOR destination)
	{
		XMFLOAT3 directionFl;
		XMStoreFloat3(&directionFl, destination);

		bottomRectanglePoint.x += directionFl.x;
		bottomRectanglePoint.y += directionFl.y;
		bottomRectanglePoint.z += directionFl.z;

		topRectanglePoint.x += directionFl.x;
		topRectanglePoint.y += directionFl.y;
		topRectanglePoint.z += directionFl.z;
	}
}