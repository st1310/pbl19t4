#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class PlatformGround1 :
		public StaticGameObject
	{
	public:
		PlatformGround1(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(0, -24.75, 0),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 90, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.5, 0.5, 0.75));
		~PlatformGround1();
	};
}