#include "GameManager.h"


namespace Rendering
{
	RTTI_DEFINITIONS(GameManager)

	GameManager::GameManager(Game& game, Camera& camera)
	{
		this->game = &game;
		this->camera = &camera;
		Initialize();

		// Start MenuLevel
		StartScene(3);
		Scenes.at(3)->Serialize();
	}


	GameManager::~GameManager()
	{
	}

	void GameManager::Initialize()
	{
		// Adding all scenes aka levels to Scenes
		MenuLevel* menuLevel = new MenuLevel(*game, *camera);
		Scenes.push_back(menuLevel);

		DayLevel* dayLevel = new DayLevel(*game, *camera);
		Scenes.push_back(dayLevel);

		TrainLevel* trainLevel = new TrainLevel(*game, *camera);
		Scenes.push_back(trainLevel);

		CityLevel* cityLevel= new CityLevel(*game, *camera);
		Scenes.push_back(cityLevel);
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
		return Scenes.at(currentScene)->GameObjects.size();
	}
}


