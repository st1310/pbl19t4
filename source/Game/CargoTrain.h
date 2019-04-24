#pragma once

#include "DrawableGameComponent.h"
#include "GameObject.h"

using namespace Library;
using namespace Rendering;

namespace Rendering
{
	class CargoTrain :
		public GameObject
	{
	public:
		CargoTrain(Game& game, Camera& camera, XMFLOAT3 position);
		~CargoTrain();
	};
}



