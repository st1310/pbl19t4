#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class OldBuilding :
		public StaticGameObject
	{
	public:
		OldBuilding(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(0, 22.5, 0),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.2, 0.2, 0.2));
		~OldBuilding();

		virtual void Initialize() override;
	};
}