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
			XMFLOAT3 startPosition = XMFLOAT3(-13.5, 12.5, 11),
			XMFLOAT3 startRotation = XMFLOAT3(180, -90, -90),
			XMFLOAT3 startScale = XMFLOAT3(0.1, 0.1, 0.1));
		~Pathcircles();

		virtual void Initialize() override;
	};
}