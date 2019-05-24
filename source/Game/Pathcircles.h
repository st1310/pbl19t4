#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class Pathcircles :
		public StaticGameObject
	{
	public:
		Pathcircles(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(0, -2.5, 0),
			XMFLOAT3 startRotation = XMFLOAT3(0, 90, 90),
			XMFLOAT3 startScale = XMFLOAT3(0.25, 0.25, 0.25));
		virtual void Initialize() override;
		~Pathcircles();
	};
}