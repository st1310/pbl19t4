#pragma once

#include "Common.h"
#include "SerializableGameObject.h"
#include "AnimatedGameObject.h"
#include "GameObject.h"
#include "AssetList.h"
#include "CollisionNode.h"

namespace Rendering
{
	class GameObjectPusher
	{
	public:
		GameObjectPusher();
		~GameObjectPusher();

		std::vector<GameComponent*> CreateAssets(Game& game, Camera& camera, std::vector<SerializableGameObject> gameObjects, bool needToFindCoord = false);
		GameObject* GetGameObjectByName(Game& game, Camera& camera, std::string className);

		std::vector<CollisionNode*> listNode;
		std::vector<DrawableGameComponent*> listOfSoldiers;

	private:
		//x - max, y i z - min
		XMFLOAT3 PosA;
		//x - min, y i z - max
		XMFLOAT3 PosC;
	};
}


