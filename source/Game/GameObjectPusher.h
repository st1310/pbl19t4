#pragma once

#include "Common.h"
#include "SerializableGameObject.h"
#include "AnimatedGameObject.h"
#include "AssetList.h"
#include "CollisionNode.h"

namespace Rendering
{
	class GameObjectPusher
	{
	public:
		GameObjectPusher();
		~GameObjectPusher();

		std::vector<GameComponent*> CreateAssets(Game& game, Camera& camera, std::vector<SerializableGameObject> gameObjects);

		std::vector<CollisionNode*> listNode;
		std::vector<GameComponent*> listOfSoldiers;
	};
}


