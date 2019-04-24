#pragma once

#include "DrawableGameComponent.h"
#include "GameObject.h"

using namespace Library;
using namespace Rendering;

namespace Rendering
{
	class PassengerTrain :
		public GameObject
	{
	public:
		PassengerTrain(Game& game, Camera& camera, XMFLOAT3 position);
		~PassengerTrain();
	};
}


