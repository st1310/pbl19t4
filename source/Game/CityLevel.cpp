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

		for (int i = 0; i < gameObjects.size(); i++)
		{
			this->GameObjects.push_back(gameObjects.at(i));
			GameObjects.at(i)->Initialize();
		}	

		GreenSoldier* soldier = new GreenSoldier(game, camera);
		//this->GameObjects.push_back(soldier);
		soldier->Initialize();
		soldier->mIsEdited = true;
		soldier->mIsSelected = true;
	}
}