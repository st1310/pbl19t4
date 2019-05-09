#pragma once

#include "Common.h"
#include "SerializableGameObject.h"
#include "GameObject.h"
#include "AssetList.h"

using namespace Library;

namespace Rendering
{
	class GameObjectPusher
	{
	public:
		GameObjectPusher();
		~GameObjectPusher();

		std::vector<GameComponent*> CreateAssets(Game& game, Camera& camera, std::vector<SerializableGameObject> gameObjects);
	};
}


