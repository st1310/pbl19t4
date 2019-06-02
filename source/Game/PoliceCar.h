#pragma once

#include "DrawableGameComponent.h"
#include "AnimatedGameObject.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class PoliceCar :
		public StaticGameObject
	{
	public:
		PoliceCar(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(0, -5.5, 0),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(12, 12, 12));
		~PoliceCar();

		virtual void Initialize() override;
	};
}