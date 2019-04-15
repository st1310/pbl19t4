#pragma once

#include "Common.h"

namespace Library
{
	class BoundingBox 
	{
	public:
		BoundingBox();
		BoundingBox(XMFLOAT3 bottom, XMFLOAT3 top);
		virtual ~BoundingBox();

		void SetBottomRect(XMFLOAT3 newBottom);
		void SetTopRect(XMFLOAT3 newTop);

		void ChangeTriggerableStatus();
		bool IsThisTrigger();
		const XMFLOAT3& BottomRect() const;
		const XMFLOAT3& TopRect() const;

		bool HasDeclaredPoints();
		bool Intersects(BoundingBox targetedBox);
		void Move(XMVECTOR destination);

	protected:
		bool isTrigger;
		XMFLOAT3 bottomRectanglePoint;
		XMFLOAT3 originalBottomPos;
		XMFLOAT3 topRectanglePoint;
		XMFLOAT3 originalTopPos;
	};
}