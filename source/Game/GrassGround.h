#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class GrassGround :
		public StaticGameObject
	{
	public:
		GrassGround(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(47, 1.75, -3),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.5, 0.5, 0.5));
		~GrassGround();
	};
}