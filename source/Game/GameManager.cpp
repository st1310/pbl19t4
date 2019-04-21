#include "GameManager.h"


namespace Rendering
{
	RTTI_DEFINITIONS(GameManager)

	GameManager::GameManager(Game& game, Camera& camera)
	{
		this->game = &game;
		this->camera = &camera;
		Init();
		StartScene(0);
	}


	GameManager::~GameManager()
	{
	}

	void GameManager::Init()
	{
		TrainLevel* trainLevel = new TrainLevel(*game, *camera);
		Scenes.push_back(trainLevel);
	}

	void GameManager::StartScene(int sceneId)
	{
		Scenes.at(sceneId)->Start(*game, *camera);
	}
}


