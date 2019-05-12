#include "CityLevel.h"

namespace Rendering
{
	CityLevel::CityLevel(Game& game, Camera& camera)
		:Scene(CITY_LEVEL, "Content\\Serializations\\city_level.data")
	{
	}

	CityLevel::~CityLevel()
	{
	}

	void CityLevel::Start(Game& game, Camera& camera)
	{
		GameObjectPusher pusher = GameObjectPusher();
		std::vector<GameComponent*> gameObjects = pusher.CreateAssets(game, camera, LoadFromFile());

		CargoTrain* g = new CargoTrain(game, camera);
		g->mIsEdited = true;
		g->mIsSelected = true;
		this->GameObjects.push_back(g);

		for (int i = 0; i < gameObjects.size(); i++)
			this->GameObjects.push_back(gameObjects.at(i));
	}
}