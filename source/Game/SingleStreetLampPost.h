#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class SingleStreetLampPost :
		public StaticGameObject
	{
	public:
		SingleStreetLampPost(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(0, -7, 0),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.15, 0.15, 0.15));
		~SingleStreetLampPost();

		virtual void Initialize() override;
	};
}