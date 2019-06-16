#pragma once

#include "GameObjectPusher.h"

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
		void SetPatrol(Policeman* policeman, int index);
	};

}


