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
			XMFLOAT3 startPosition = XMFLOAT3(0, -17, 0),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.5, 0.5, 0.5));
		~PlatformGround1();
	};
}