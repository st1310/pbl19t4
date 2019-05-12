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
			std::string className = gameObjects.at(i).GetAssetClassName();

			if (className == "CargoTrain")
			{
				CargoTrain* cargoTrain = new CargoTrain(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				assets.push_back(cargoTrain);
			}

			if (className == "PassengerTrain")
			{
				PassengerTrain* passengerTrain = new PassengerTrain(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				assets.push_back(passengerTrain);
			}

			if (className == "GreenSoldier")
			{
				GreenSoldier* greenSoldier = new GreenSoldier(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				assets.push_back(greenSoldier);
			}

			if (className == "Policeman")
			{
				Policeman* policeman = new Policeman(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				assets.push_back(policeman);
			}

			if (className == "Track")
			{
				Track* track = new Track(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				assets.push_back(track);
			}

			if (className == "Bench")
			{
				Bench* bench = new Bench(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				assets.push_back(bench);
			}

			if (className == "ConcreteWall")
			{
				ConcreteWall* concreteWall = new ConcreteWall(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				assets.push_back(concreteWall);
			}

			if (className == "StoneGround")
			{
				StoneGround* stoneGround = new StoneGround(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				assets.push_back(stoneGround);
			}

			if (className == "GrassGround")
			{
				GrassGround* grassGround = new GrassGround(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				assets.push_back(grassGround);
			}

			if (className == "PlatformGround1")
			{
				PlatformGround1* platformGround1 = new PlatformGround1(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				assets.push_back(platformGround1);
			}

			if (className == "PlatformGround2")
			{
				PlatformGround2* platformGround2 = new PlatformGround2(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				assets.push_back(platformGround2);
			}

			if (className == "DoubleStreetLampPost")
			{
				DoubleStreetLampPost* doubleStreetLampPost = new DoubleStreetLampPost(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				assets.push_back(doubleStreetLampPost);
			}

			if (className == "DoubleStreetLampPostWithMegaphone")
			{
				DoubleStreetLampPostWithMegaphone* doubleStreetLampPostWithMegaphone = new DoubleStreetLampPostWithMegaphone(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				assets.push_back(doubleStreetLampPostWithMegaphone);
			}

			if (className == "SingleStreetLampPost")
			{
				SingleStreetLampPost* singleStreetLampPost = new SingleStreetLampPost(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				assets.push_back(singleStreetLampPost);
			}
		}

		return assets;
	}

	GameObject* GameObjectPusher::GetGameObjectByName(Game& game, Camera& camera, std::string className)
	{
		if (className == "CargoTrain")
		{
			CargoTrain* cargoTrain = new CargoTrain(game, camera);
			return cargoTrain;
		}

		if (className == "PassengerTrain")
		{
			PassengerTrain* passengerTrain = new PassengerTrain(game, camera);
			return passengerTrain;
		}

		if (className == "GreenSoldier")
		{
			GreenSoldier* greenSoldier = new GreenSoldier(game, camera);
			return greenSoldier;
		}

		if (className == "Policeman")
		{
			//Policeman* policeman = new Policeman(game, camera);
			Bench* policeman = new Bench(game, camera);
			return policeman;
		}

		if (className == "Track")
		{
			Track* track = new Track(game, camera);
			return track;
		}

		if (className == "Bench")
		{
			Bench* bench = new Bench(game, camera);
			return bench;
		}

		if (className == "ConcreteWall")
		{
			ConcreteWall* concreteWall = new ConcreteWall(game, camera);
			return concreteWall;
		}

		if (className == "StoneGround")
		{
			StoneGround* stoneGround = new StoneGround(game, camera);
			return stoneGround;
		}

		if (className == "GrassGround")
		{
			GrassGround* grassGround = new GrassGround(game, camera);
			return grassGround;
		}

		if (className == "PlatformGround1")
		{
			PlatformGround1* platformGround1 = new PlatformGround1(game, camera);
			return platformGround1;
		}

		if (className == "PlatformGround2")
		{
			PlatformGround2* platformGround2 = new PlatformGround2(game, camera);
			return platformGround2;
		}

		if (className == "DoubleStreetLampPost")
		{
			DoubleStreetLampPost* doubleStreetLampPost = new DoubleStreetLampPost(game, camera);
			return doubleStreetLampPost;
		}

		if (className == "DoubleStreetLampPostWithMegaphone")
		{
			DoubleStreetLampPostWithMegaphone* doubleStreetLampPostWithMegaphone = new DoubleStreetLampPostWithMegaphone(game, camera);
			return doubleStreetLampPostWithMegaphone;
		}

		if (className == "SingleStreetLampPost")
		{
			SingleStreetLampPost* singleStreetLampPost = new SingleStreetLampPost(game, camera);
			return singleStreetLampPost;
		}

		else
		{
			Bench* bench = new Bench(game, camera);
			return bench;
		}
	}
}