#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class SimpleBuilding :
		public StaticGameObject
	{
	public:
		SimpleBuilding(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(36, -24.75, -149),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 90, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.4, 0.4, 0.4));
		~SimpleBuilding();
	};
}