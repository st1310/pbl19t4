#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class GrassGroundNew :
		public StaticGameObject
	{
	public:
		GrassGroundNew(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(0, -11.75, 0), // it's so fuckin bugged and multiple by 2 -> valid (23, -11.6, 0)
			XMFLOAT3 startRotation = XMFLOAT3(-90, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.5, 0.5, 0.5));
		~GrassGroundNew();
	};
}