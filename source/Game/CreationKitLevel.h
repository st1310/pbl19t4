#pragma once

#include "GameObjectPusher.h"

namespace Rendering
{
	class CreationKitLevel :
		public Scene
	{
	public:
		CreationKitLevel(Game& game, Camera& camera);
		~CreationKitLevel();

		virtual void Start(Game& game, Camera& camera) override;
	};
}