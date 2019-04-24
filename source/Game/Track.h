#pragma once

#include "DrawableGameComponent.h"
#include "GameObject.h"

using namespace Library;
using namespace Rendering;

namespace Rendering
{
	class Track :
		public GameObject
	{
	public:
		Track(Game& game, Camera& camera, XMFLOAT3 position);
		~Track();
	};
}

