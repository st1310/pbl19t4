#include "GameObjectPusher.h"
#include "NodeList.h"
#include "Pathcircles.h"

namespace Rendering
{
	GameObjectPusher::GameObjectPusher() :
		listNode(), listOfSoldiers(), triggerableObjects()
	{
	}


	GameObjectPusher::~GameObjectPusher()
	{
		listNode.clear();
		listOfSoldiers.clear();
		triggerableObjects.clear();
	}

	std::vector<GameComponent*> GameObjectPusher::CreateAssets(Game& game, Camera& camera, std::vector<SerializableGameObject> gameObjects, bool needToFindCoord)
	{
		std::vector<GameComponent*> assets = std::vector<GameComponent*>();

		for (int i = 0; i < gameObjects.size(); i++)
		{
			if (needToFindCoord)
			{
				PosA = XMFLOAT3(0.f, 0.f, 0.f);
				PosC = XMFLOAT3(0.f, 0.f, 0.f);
				XMFLOAT3 posit = gameObjects.at(i).GetPosition();

				if (posit.x > PosA.x)
					PosA.x = posit.x;
				if (posit.x < PosC.x)
					PosC.x = posit.x;

				if (posit.y < PosA.y)
					PosA.y = posit.y;
				if (posit.y > PosC.y)
					PosC.y = posit.y;

				if (posit.z < PosA.z)
					PosA.z = posit.z;
				if (posit.z > PosC.z)
					PosC.z = posit.z;
			}

			std::string className = gameObjects.at(i).GetAssetClassName();

			if (className == "Bench")
			{
				Bench* bench = new Bench(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				bench->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(bench);
			}

			if (className == "CargoTrain")
			{
				CargoTrain* cargoTrain = new CargoTrain(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				cargoTrain->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(cargoTrain);
			}

			if (className == "ConcreteWall")
			{
				ConcreteWall* concreteWall = new ConcreteWall(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				concreteWall->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));

				assets.push_back(concreteWall);
			}

			if (className == "DoubleStreetLampPost")
			{
				DoubleStreetLampPost* doubleStreetLampPost = new DoubleStreetLampPost(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				doubleStreetLampPost->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(doubleStreetLampPost);
			}

			if (className == "DoubleStreetLampPostWithMegaphone")
			{
				DoubleStreetLampPostWithMegaphone* doubleStreetLampPostWithMegaphone = new DoubleStreetLampPostWithMegaphone(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				doubleStreetLampPostWithMegaphone->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(doubleStreetLampPostWithMegaphone);
			}

			if (className == "FarbaMan")
			{
				FarbaMan* farbaMan = new FarbaMan(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				farbaMan->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				triggerableObjects.push_back(farbaMan);
				listOfSoldiers.push_back(farbaMan);
				assets.push_back(farbaMan);
			}

			if (className == "GrassGround")
			{
				GrassGround* grassGround = new GrassGround(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				grassGround->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(grassGround);
			}

			if (className == "GrassGroundNew")
			{
				GrassGroundNew* grassGroundNew = new GrassGroundNew(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				grassGroundNew->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(grassGroundNew);
			}

			if (className == "Soldier")
			{
				GreenSoldier* greenSoldier = new GreenSoldier(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				greenSoldier->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(greenSoldier);
				triggerableObjects.push_back(greenSoldier);
				listOfSoldiers.push_back(greenSoldier);
			}

			if (className == "Information")
			{
				Information* information = new Information(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				information->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(information);
			}

			if (className == "InformationBoard")
			{
				InformationBoard* informationBoard = new InformationBoard(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				informationBoard->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(informationBoard);
			}

			if (className == "OverheadLines")
			{
				OverheadLines* overheadLines = new OverheadLines(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				overheadLines->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(overheadLines);
			}

			if (className == "PassengerTrain")
			{
				PassengerTrain* passengerTrain = new PassengerTrain(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				passengerTrain->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(passengerTrain);
			}

			if (className == "Pathcircles")
			{
				Pathcircles* pathcircles = new Pathcircles(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				pathcircles->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(pathcircles);
			}

			if (className == "PlatformGround1")
			{
				PlatformGround1* platformGround1 = new PlatformGround1(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				platformGround1->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(platformGround1);
			}

			if (className == "PlatformGround2")
			{
				PlatformGround2* platformGround2 = new PlatformGround2(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				platformGround2->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(platformGround2);
			}

			if (className == "PlatformNumber1")
			{
				PlatformNumber1* platformNumber1 = new PlatformNumber1(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				platformNumber1->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(platformNumber1);
			}

			if (className == "PlatformNumber2And3")
			{
				PlatformNumber2And3* platformNumber2And3 = new PlatformNumber2And3(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				platformNumber2And3->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(platformNumber2And3);
			}

			if (className == "PlatformNumber4")
			{
				PlatformNumber4* platformNumber4 = new PlatformNumber4(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				platformNumber4->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(platformNumber4);
			}

			if (className == "PoliceCar")
			{
				PoliceCar* policeCar = new PoliceCar(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				policeCar->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(policeCar);
			}

			if (className == "Policeman")
			{
				Policeman* policeman = new Policeman(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				policeman->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				triggerableObjects.push_back(policeman);
				assets.push_back(policeman);
			}

			if (className == "SingleStreetLampPost")
			{
				SingleStreetLampPost* singleStreetLampPost = new SingleStreetLampPost(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				singleStreetLampPost->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(singleStreetLampPost);
			}

			if (className == "StoneGround")
			{
				StoneGround* stoneGround = new StoneGround(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				stoneGround->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(stoneGround);
			}

			if (className == "Track")
			{
				Track* track = new Track(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				track->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(track);
			}
		}

		return assets;
	}

	GameObject* GameObjectPusher::GetGameObjectByName(Game& game, Camera& camera, std::string className)
	{
		if (className == "Bench")
		{
			Bench* bench = new Bench(game, camera);
			return bench;
		}

		if (className == "CargoTrain")
		{
			CargoTrain* cargoTrain = new CargoTrain(game, camera);
			return cargoTrain;
		}

		if (className == "ConcreteWall")
		{
			ConcreteWall* concreteWall = new ConcreteWall(game, camera);
			return concreteWall;
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

		if (className == "FarbaMan")
		{
			FarbaMan* farbaMan = new FarbaMan(game, camera);
			return farbaMan;
		}

		if (className == "GrassGround")
		{
			GrassGround* grassGround = new GrassGround(game, camera);
			return grassGround;
		}

		if (className == "GrassGroundNew")
		{
			GrassGroundNew* grassGroundNew = new GrassGroundNew(game, camera);
			return grassGroundNew;
		}

		if (className == "Soldier")
		{
			GreenSoldier* greenSoldier = new GreenSoldier(game, camera);
			return greenSoldier;
		}

		if (className == "Information")
		{
			Information* information = new Information(game, camera);
			return information;
		}

		if (className == "InformationBoard")
		{
			InformationBoard* informationBoard = new InformationBoard(game, camera);
			return informationBoard;
		}

		if (className == "OverheadLines")
		{
			OverheadLines* overheadLines = new OverheadLines(game, camera);
			return overheadLines;
		}

		if (className == "PassengerTrain")
		{
			PassengerTrain* passengerTrain = new PassengerTrain(game, camera);
			return passengerTrain;
		}

		if (className == "Pathcircles")
		{
			Pathcircles* pathcircles = new Pathcircles(game, camera);
			return pathcircles;
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

		if (className == "PlatformNumber1")
		{
			PlatformNumber1* platformNumber1 = new PlatformNumber1(game, camera);
			return platformNumber1;
		}

		if (className == "PlatformNumber2And3")
		{
			PlatformNumber2And3* platformNumber2And3 = new PlatformNumber2And3(game, camera);
			return platformNumber2And3;
		}

		if (className == "PlatformNumber4")
		{
			PlatformNumber4* platformNumber4 = new PlatformNumber4(game, camera);
			return platformNumber4;
		}

		if (className == "PoliceCar")
		{
			PoliceCar* policeCar = new PoliceCar(game, camera);
			return policeCar;
		}

		if (className == "Policeman")
		{
			Policeman* policeman = new Policeman(game, camera);
			return policeman;
		}

		if (className == "SingleStreetLampPost")
		{
			SingleStreetLampPost* singleStreetLampPost = new SingleStreetLampPost(game, camera);
			return singleStreetLampPost;
		}

		if (className == "StoneGround")
		{
			StoneGround* stoneGround = new StoneGround(game, camera);
			return stoneGround;
		}

		if (className == "Track")
		{
			Track* track = new Track(game, camera);
			return track;
		}

		else
		{
			Bench* bench = new Bench(game, camera);
			return bench;
		}
	}

	XMFLOAT3 GameObjectPusher::GetPosA()
	{
		return PosA;
	}

	XMFLOAT3 GameObjectPusher::GetPosC()
	{
		return PosC;
	}
}