#pragma once

#include "GameObjectPusher.h"

using namespace Library;

namespace Rendering
{
	class DayLevel :
		public Scene
	{
	public:
		DayLevel(Game& game, Camera& camera);
		~DayLevel();

		virtual void Start(Game& game, Camera& camera) override;
	};
}
