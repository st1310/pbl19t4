#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class PlatformGround2 :
		public StaticGameObject
	{
	public:
		PlatformGround2(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(36, -17, 0),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.5, 0.5, 0.5));
		~PlatformGround2();
	};
}