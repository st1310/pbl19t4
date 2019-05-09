#pragma once
#include "Scene.h"
#include "MenuLevel.h"
#include "DayLevel.h"
#include "TrainLevel.h"
#include "CityLevel.h"

using namespace Library;


namespace Rendering
{
	class GameManager :
		public DrawableGameComponent
	{
		RTTI_DECLARATIONS(GameManager, DrawableGameComponent)

	public:
		GameManager(Game& game, Camera& camera);
		~GameManager();
		void Initialize();
		void StartScene(int sceneId);
		int GetSizeOfCurrentScene();

		std::vector<Scene*> Scenes;
		int currentScene;
	private:
		
		Game* game;
		Camera* camera;
		GameManager(const GameManager& rhs);
		GameManager& operator=(const GameManager& rhs);
	};
}


