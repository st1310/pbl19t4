#pragma once

#include "DrawableGameComponent.h"
#include "Scene.h"
#include "Earth.h"
#include "CargoTrain.h"
#include "PassengerTrain.h"
#include "Track.h"
#include "GreenSoldier.h"

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


