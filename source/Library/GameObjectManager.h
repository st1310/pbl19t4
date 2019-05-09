#pragma once

#include <iostream>
#include <list>
#include "Scene.h"
//#include "TrainLevel.h"


using namespace Library;

namespace Rendering
{
	class GameObjectManager
	{
	public:
		GameObjectManager(Game& game, Camera& camera);
		~GameObjectManager();
		Scene GetScene(int sceneId);
		

	private:
		void Init(Game& game, Camera& camera);
		Scene Scenes [5];
		
	};
}



