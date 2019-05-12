#include "TrainLevel.h"


namespace Rendering
{
	TrainLevel::TrainLevel(Game& game, Camera& camera)
		:Scene(TRAIN_LEVEL, "Content\\Serializations\\train_level.data")
	{
	}


	TrainLevel::~TrainLevel()
	{
	}

	void TrainLevel::Start(Game& game, Camera& camera)
	{
		GameObjectPusher pusher = GameObjectPusher();
		std::vector<GameComponent*> gameObjects = pusher.CreateAssets(game, camera, LoadFromFile());

		for (int i = 0; i < gameObjects.size(); i++)
			this->GameObjects.push_back(gameObjects.at(i));
	}
}