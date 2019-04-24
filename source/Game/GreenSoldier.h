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
		GreenSoldier(Game& game, Camera& camera, XMFLOAT4 position);
		~GreenSoldier();
	};
}



