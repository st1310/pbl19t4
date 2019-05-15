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
		pusher.listNode = this->getListOfNode();
		std::vector<GameComponent*> gameObjects = pusher.CreateAssets(game, camera, LoadFromFile());
		
		RewriteUnitList(pusher.listOfSoldiers);


		for (int i = 0; i < gameObjects.size(); i++)
			this->GameObjects.push_back(gameObjects.at(i));
			
	}
}