#pragma once

#include "DrawableGameComponent.h"
#include "TexturedModelDemo.h"

using namespace Library;
using namespace Rendering;

namespace Rendering
{
	class Track :
		public TexturedModelDemo
	{
	public:
		Track(Game& game, Camera& camera, double x, double y);
		~Track();
	};
}

