#pragma once
#include "GameCamera.h"
#include "Scene.h"
#include "MenuLevel.h"
#include "DayLevel.h"
#include "TrainLevel.h"
#include "CityLevel.h"
#include "CreationKitLevel.h"
#include "PathFinder_Test.h"
#include "Button.h"
#include "GUI.h"

namespace Rendering
{
		class PathFinding_Test;
		class GUI;

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
			void SelectingGroundsFake(long mouseX, long mouseY);
			void SelectingGroundsFakePatrolMode(long mouseX, long mouseY);
			int GetCurrentSceneId();
			bool GetunitsReadyToMove();
			bool GetShowMousePosition();
			bool GetrenderGameFarbaManSpawnFlag();
			bool GetPaintButtonFlag();

			void SelectingUnits(float mouseX, float mouseY);
			void SelectingUnits(float mouse1X, float mouse1Y, float mouse2X, float mouse2Y);

			void SetKeyboard(KeyboardComponent* newKeyboard);

			std::vector<DrawableGameComponent*> GetListOfUnits();
			int unitID = -1;
		private:
			Game * game;
			Camera* camera;
			GameManager(const GameManager& rhs);
			GameManager& operator=(const GameManager& rhs);

			std::vector<Scene*> mScenes;
			int mCurrentScene;

			int GetSizeOfCurrentScene();
			bool unitsReadyToMove;

			KeyboardComponent* mKeyboard;
			//tests-remove after
			bool ShowMousePosition;
			XMFLOAT3 targetPos;
			PathFinding* pathfinding;

			//int Painted;

			std::vector<Button*> guiButtons;
			std::vector<XMFLOAT2> patrolPositions;
			std::vector<Trace1*> splashes;
			bool achiveFarbaMan = false;
			bool renderGameFarbaManSpawnFlag = false;
			bool PaintButtonFlag = false;
			bool PaintFinishedFlag = false;
		};
}


