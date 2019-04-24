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
		Earth(Game& game, Camera& camera, XMFLOAT4 position);
		~Earth();
	};
}


