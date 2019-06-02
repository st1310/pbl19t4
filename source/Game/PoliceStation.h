#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class PoliceStation :
		public StaticGameObject
	{
	public:
		PoliceStation(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(0, 34, 0),
			XMFLOAT3 startRotation = XMFLOAT3(180, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.1, 0.1, 0.1));
		~PoliceStation();

		virtual void Initialize() override;
	};
}