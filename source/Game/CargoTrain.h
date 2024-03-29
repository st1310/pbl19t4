#pragma once

#include "DrawableGameComponent.h"
#include "TexturedModelDemo.h"

using namespace Library;
using namespace Rendering;

namespace Rendering
{
	class CargoTrain :
		public TexturedModelDemo
	{
	public:
		CargoTrain(Game& game, Camera& camera, double x, double y);
		~CargoTrain();
	};
}



