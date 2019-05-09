#include "GameObjectPusher.h"


namespace Rendering
{
	GameObjectPusher::GameObjectPusher()
	{
	}


	GameObjectPusher::~GameObjectPusher()
	{
	}

	std::vector<GameComponent*> GameObjectPusher::CreateAssets(Game& game, Camera& camera, std::vector<SerializableGameObject> gameObjects)
	{
		std::vector<GameComponent*> assets = std::vector<GameComponent*>();

		for (int i = 0; i < gameObjects.size(); i++)
		{
			std::string className = gameObjects.at(i).className;

			if (className == "CargoTrain")
			{
				CargoTrain* cargoTrain = new CargoTrain(game, camera,
					gameObjects.at(i).position,
					gameObjects.at(i).rotation,
					gameObjects.at(i).scale);

				assets.push_back(cargoTrain);
			}

			if (className == "PassengerTrain")
			{
				PassengerTrain* passengerTrain = new PassengerTrain(game, camera,
					gameObjects.at(i).position,
					gameObjects.at(i).rotation,
					gameObjects.at(i).scale);

				assets.push_back(passengerTrain);
			}
			className.size();

			if (className == "GreenSoldier")
			{
				GreenSoldier* greenSoldier = new GreenSoldier(game, camera,
					gameObjects.at(i).position,
					gameObjects.at(i).rotation,
					gameObjects.at(i).scale);

				assets.push_back(greenSoldier);
			}

			if (className == "Track")
			{
				Track* track = new Track(game, camera,
					gameObjects.at(i).position,
					gameObjects.at(i).rotation,
					gameObjects.at(i).scale);

				assets.push_back(track);
			}
		}

		return assets;
	}
}

