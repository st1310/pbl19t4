#include "GameManager.h"


namespace Rendering
{
	GameManager::GameManager(Game& game, Camera& camera)
	{
		this->game = &game;
		this->camera = &camera;
		Init();
		StartScene(2);
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
		Scenes.at(0)->Start(*game, *camera);
	}
}


