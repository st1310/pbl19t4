#pragma once

#include "GameObjectPusher.h"

using namespace Library;


namespace Rendering
{
	class TrainLevel :
		public Scene
	{
	public:
		TrainLevel(Game& game, Camera& camera);
		~TrainLevel();
		
		void Start(Game& game, Camera& camera);

	private:
		
	};

}


