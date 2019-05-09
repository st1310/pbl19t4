#pragma once

#include "DrawableGameComponent.h"
#include "GameObject.h"

using namespace Library;
using namespace Rendering;

namespace Rendering
{
	class GreenSoldier :
		public GameObject
	{
	public:
		GreenSoldier(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(0, 0, 0),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.05, 0.05, 0.05));
		~GreenSoldier();
	};
}


