#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class InformationBoard :
		public StaticGameObject
	{
	public:
		InformationBoard(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(-11, -9.75, 0),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 180, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.4, 0.4, 0.4));
		~InformationBoard();
	};
}