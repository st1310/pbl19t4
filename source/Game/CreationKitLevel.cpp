#include "CreationKitLevel.h"

namespace Rendering
{
	CreationKitLevel::CreationKitLevel(Game& game, Camera& camera)
		:Scene(CREATION_KIT_LEVEL, "Content\\Serializations\\creation_kit_level.data")
	{
	}


	CreationKitLevel::~CreationKitLevel()
	{
	}


	void CreationKitLevel::Start(Game& game, Camera& camera)
	{
		GameObjectPusher pusher = GameObjectPusher();
		std::vector<GameComponent*> gameObjects = pusher.CreateAssets(game, camera, LoadFromFile());

		for (int i = 0; i < gameObjects.size(); i++)
		{
			this->GameObjects.push_back(gameObjects.at(i));
			GameObjects.at(i)->Initialize();
		}
	}
}