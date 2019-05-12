#include "GameManager.h"


namespace Rendering
{
	RTTI_DEFINITIONS(GameManager)

	GameManager::GameManager(Game& game, Camera& camera)
	{
		this->game = &game;
		this->camera = &camera;
		Initialize();
		
		//currentScene = MENU_LEVEL;
		currentScene = CITY_LEVEL;
		StartScene(currentScene);
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

		CreationKitLevel* creationKitLevel = new CreationKitLevel(*game, *camera);
		Scenes.push_back(creationKitLevel);
	}

	void GameManager::StartScene(int sceneId)
	{
		// Clear old scene
		Scenes.at(currentScene)->Clear();

		currentScene = sceneId;

		// Fix scene Start 
		Scenes.at(sceneId)->Start(*game, *camera);	
	}

	int GameManager::GetSizeOfCurrentScene()
	{
		return Scenes.at(currentScene)->GameObjects.size();
	}

	void GameManager::Update(const GameTime& gameTime)
	{
		for(int i =0; i <  GetSizeOfCurrentScene(); i++)
			Scenes[currentScene]->GameObjects[i]->Update(gameTime);
	}

	void GameManager::Draw(const GameTime& gameTime)
	{
		for (GameComponent* component : Scenes[currentScene]->GameObjects)
		{
			DrawableGameComponent* drawableGameComponent = component->As<DrawableGameComponent>();
			if (drawableGameComponent != nullptr)
				drawableGameComponent->Draw(gameTime);
		}
	}
}


