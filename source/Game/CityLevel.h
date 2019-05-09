#pragma once

#include "GameObjectPusher.h"

using namespace Library;

namespace Rendering
{
	class CityLevel :
		public Scene
	{
	public:
		CityLevel(Game& game, Camera& camera);
		~CityLevel();

		virtual void Start(Game& game, Camera& camera) override;
	};
}