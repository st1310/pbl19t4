#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class TrainStationMap :
		public StaticGameObject
	{
	public:
		TrainStationMap(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(23.5, -9, -35.75),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.05, 0.05, 0.05));
		~TrainStationMap();
	};
}
