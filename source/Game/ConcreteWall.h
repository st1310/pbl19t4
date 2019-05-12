#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class ConcreteWall :
		public StaticGameObject
	{
	public:
		ConcreteWall(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(0, 8.5, 0),
			XMFLOAT3 startRotation = XMFLOAT3(90, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.5, 0.25, 0.3));
		~ConcreteWall();
	};
}