#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class Barrier :
		public StaticGameObject
	{
	public:
		Barrier(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(244.5, -9.75, 395.5),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 180, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.5, 0.15, 0.5));
		~Barrier();
	};
}