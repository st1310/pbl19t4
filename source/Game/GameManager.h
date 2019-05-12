#pragma once
#include "Scene.h"
#include "MenuLevel.h"
#include "DayLevel.h"
#include "TrainLevel.h"
#include "CityLevel.h"
#include "CreationKitLevel.h"

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


