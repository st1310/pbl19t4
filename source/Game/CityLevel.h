#pragma once

#include "AssetList.h"

using namespace Library;

namespace Rendering
{
	class CityLevel :
		public Scene
	{
	public:
		CityLevel(Game& game, Camera& camera);
		~CityLevel();

		void Start(Game& game, Camera& camera);
	};
}