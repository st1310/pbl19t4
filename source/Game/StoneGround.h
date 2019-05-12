#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class StoneGround :
		public StaticGameObject
	{
	public:
		StoneGround(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(45, 4.5, 0),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.5, 0.5, 0.5));
		~StoneGround();
	};
}