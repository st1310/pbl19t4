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
		
		virtual void Start(Game& game, Camera& camera) override;

	private:
		
	};

}


