#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class Track :
		public StaticGameObject
	{
	public:
		Track(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(0, 0, 0),
			XMFLOAT3 startRotation = XMFLOAT3(0, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(1, 1, 1));
		~Track();
	};
}