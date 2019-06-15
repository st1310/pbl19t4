#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class Fence :
		public StaticGameObject
	{
	public:
		Fence(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(36, -24.75, -149),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 90, 90),
			XMFLOAT3 startScale = XMFLOAT3(0.15, 0.15, 0.15));
		~Fence();
	};
}