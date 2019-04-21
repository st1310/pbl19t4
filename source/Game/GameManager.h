#pragma once
#include "Scene.h"
#include "TrainLevel.h"
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
		void Init();
		void StartScene(int sceneId);

		std::vector<Scene*> Scenes;

	private:
		int currentScene;
		Game* game;
		Camera* camera;
		GameManager(const GameManager& rhs);
		GameManager& operator=(const GameManager& rhs);
	};
}


