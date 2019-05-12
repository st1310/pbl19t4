#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class CargoTrain :
		public StaticGameObject
	{
	public:
		CargoTrain(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(0, 9.6, 0),
			XMFLOAT3 startRotation = XMFLOAT3(180, -90, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.2, 0.2, 0.2));
		~CargoTrain();
	};
}