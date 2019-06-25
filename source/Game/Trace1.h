#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class Trace1 :
		public StaticGameObject
	{
	public:
		Trace1(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(2, -9, 1),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.2, 0.2, 0.2));
		~Trace1();

	};
}