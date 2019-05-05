#include "DayLevel.h"

namespace Rendering
{
	DayLevel::DayLevel(Game& game, Camera& camera)
		:Scene(DAY_LEVEL, "day_level.txt")
	{
		Start(game, camera);
	}


	DayLevel::~DayLevel()
	{
	}

	void DayLevel::Start(Game& game, Camera& camera)
	{
		GameObjectPusher pusher = GameObjectPusher();
		std::vector<GameComponent*> gameObjects = pusher.CreateAssets(game, camera, LoadFromFile());

		for (int i = 0; i < gameObjects.size(); i++)
			this->GameObjects.push_back(gameObjects.at(i));

	}
}


