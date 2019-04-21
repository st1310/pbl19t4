#pragma once

#include "DrawableGameComponent.h"
#include "Scene.h"
#include "Earth.h"
#include "CargoTrain.h"
#include "PassengerTrain.h"
#include "Track.h"

using namespace Library;

namespace Rendering
{
	class MenuLevel :
		public Scene
	{
	public:
		MenuLevel(Game& game, Camera& camera);
		~MenuLevel();

		void Start(Game& game, Camera& camera);
	};
}


