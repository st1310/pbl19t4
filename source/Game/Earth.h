#pragma once

#include "DrawableGameComponent.h"
#include "GameObject.h"

using namespace Library;
using namespace Rendering;

namespace Rendering
{
	class Earth :
		public GameObject
	{
	public:
		Earth(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(0, 0, 0),
			XMFLOAT3 startRotation = XMFLOAT3(0, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(1, 1, 1));
		~Earth();
	};
}


