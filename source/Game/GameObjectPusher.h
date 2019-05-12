#pragma once

#include "Common.h"
#include "SerializableGameObject.h"
#include "AnimatedGameObject.h"
#include "GameObject.h"
#include "AssetList.h"

namespace Rendering
{
	class GameObjectPusher
	{
	public:
		GameObjectPusher();
		~GameObjectPusher();

		std::vector<GameComponent*> CreateAssets(Game& game, Camera& camera, std::vector<SerializableGameObject> gameObjects);
		GameObject* GetGameObjectByName(Game& game, Camera& camera, std::string className);
	};
}


