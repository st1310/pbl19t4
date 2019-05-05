#pragma once

#include "GameObjectPusher.h"

using namespace Library;

namespace Rendering
{
	class MenuLevel :
		public Scene
	{
	public:
		MenuLevel(Game& game, Camera& camera);
		~MenuLevel();

		void Start(Game& game, Camera& camera);
	};
}


