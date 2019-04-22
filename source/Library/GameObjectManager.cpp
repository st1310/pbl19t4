#include "GameObjectManager.h"

const int MENU = 0;
const int TRAIN_LEVEL = 1;
const int CITY_LEVEL = 2;


namespace Rendering
{
	GameObjectManager::GameObjectManager(Game& game, Camera& camera)
	{
		Init(game, camera);
	}


	GameObjectManager::~GameObjectManager()
	{
	}

	void GameObjectManager::Init(Game& game, Camera& camera)
	{		
		//Scene trainLevel = TrainLevel(TRAIN_LEVEL, game, camera);
		
		//Scenes[TRAIN_LEVEL] = trainLevel;
	}

	Scene GameObjectManager::GetScene(int sceneId)
	{
		return Scenes[sceneId];
	}
}


