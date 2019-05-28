#pragma once
#include "FirstPersonCamera.h"
#include "Scene.h"
#include "MenuLevel.h"
#include "DayLevel.h"
#include "TrainLevel.h"
#include "CityLevel.h"
#include "CreationKitLevel.h"
#include "PathFinder_Test.h"

namespace Rendering
{
	class GameManager :
		public DrawableGameComponent
	{
		RTTI_DECLARATIONS(GameManager, DrawableGameComponent)

	public:
		GameManager(Game& game, Camera& camera);
		~GameManager();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		void StartScene(int sceneId);
		std::vector<CollisionNode*> GetCurrentListOfNodes();
		void SelectingGrounds(long mouseX, long mouseY);
		int GetCurrentSceneId();

		void SelectingUnits(float mouseX, float mouseY);
		void SelectingUnits(float mouse1X, float mouse1Y, float mouse2X, float mouse2Y);
	private:
		Game* game;
		Camera* camera;
		GameManager(const GameManager& rhs);
		GameManager& operator=(const GameManager& rhs);

		std::vector<Scene*> mScenes;
		int mCurrentScene;

		int GetSizeOfCurrentScene();

	};
}


