#include "DayLevel.h"

namespace Rendering
{
	DayLevel::DayLevel(Game& game, Camera& camera)
		:Scene(game, camera,DAY_LEVEL, "Content\\Serializations\\day_level.data")
	{
	}

	DayLevel::~DayLevel()
	{
	}

	void DayLevel::Start(Game& game, Camera& camera)
	{
		Initialize();
		GameObjectPusher pusher = GameObjectPusher();
		pusher.listNode = this->getListOfNode();
		std::vector<GameComponent*> gameObjects = pusher.CreateAssets(game, camera, LoadFromFile());

		for (int i = 0; i < gameObjects.size(); i++)
		{
			this->GameObjects.push_back(gameObjects.at(i));
			GameObjects.at(i)->Initialize();
		}
	}
}