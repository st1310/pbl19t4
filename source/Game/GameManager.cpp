#include "GameManager.h"


namespace Rendering
{
	RTTI_DEFINITIONS(GameManager)

	GameManager::GameManager(Game& game, Camera& camera)
	{
		this->game = &game;
		this->camera = &camera;
		Init();

		// Start MenuLevel
		StartScene(0);
	}


	GameManager::~GameManager()
	{
	}

	void GameManager::Init()
	{
		// Adding all scenes aka levels to Scenes
		MenuLevel* menuLevel = new MenuLevel(*game, *camera);
		Scenes.push_back(menuLevel);

		TrainLevel* trainLevel = new TrainLevel(*game, *camera);
		Scenes.push_back(trainLevel);
	}

	void GameManager::StartScene(int sceneId)
	{
		// Clear old scene
		//Scenes.at(sceneId)->Clear();

		currentScene = sceneId;

		// Fix scene Start 
		Scenes.at(sceneId)->Start(*game, *camera);	
	}

	int GameManager::GetSizeOfCurrentScene()
	{
		return Scenes.at(currentScene)->Size;
	}
}


