#pragma once
#include "Scene.h"
#include "MenuLevel.h"
#include "DayLevel.h"
#include "TrainLevel.h"
#include "CityLevel.h"
#include "CreationKitLevel.h"
#include "AssetList.h"

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
		std::vector<CollisionNode*> GetCurrentListOfNodes();
		void SelectingUnits(XMVECTOR camOr, XMVECTOR ray, float dist, bool selectSeveral);

		int currentScene;
	private:
		
		Game* game;
		Camera* camera;
		GameManager(const GameManager& rhs);
		GameManager& operator=(const GameManager& rhs);
	};
}


