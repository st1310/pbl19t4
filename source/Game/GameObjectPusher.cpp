#include "GameObjectPusher.h"
#include "NodeList.h"
#include "Pathcircles.h"
#include "ModelList.h"

namespace Rendering
{
	GameObjectPusher::GameObjectPusher() :
		listNode(), listOfSoldiers(), triggerableObjects()
	{
		mModList = nullptr;
	}


	GameObjectPusher::~GameObjectPusher()
	{
		listNode.clear();
		listOfSoldiers.clear();
		triggerableObjects.clear();
	}

	std::vector<GameComponent*> GameObjectPusher::CreateAssets(Game& game, Camera& camera, std::vector<SerializableGameObject> gameObjects, bool needToFindCoord)
	{
		if (mModList == nullptr)
		{
			mModList = new ModelList(game);
		}
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

			if (className == "Barrier")
			{
				Barrier* barrier = new Barrier(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				barrier->SetModel(mModList->FindModel("Barrier"));
				barrier->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(barrier);
			}

			if (className == "Bench")
			{
				Bench* bench = new Bench(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				bench->SetModel(mModList->FindModel("Bench"));
				bench->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(bench);
			}

			if (className == "CargoTrain")
			{
				CargoTrain* cargoTrain = new CargoTrain(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				cargoTrain->SetModel(mModList->FindModel("CargoTrain"));
				cargoTrain->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(cargoTrain);
			}

			if (className == "ConcreteWall")
			{
				ConcreteWall* concreteWall = new ConcreteWall(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				concreteWall->SetModel(mModList->FindModel("ConcreteWall"));
				concreteWall->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));

				assets.push_back(concreteWall);
			}

			if (className == "TrainStationMap")
			{
				TrainStationMap* trainStationMap = new TrainStationMap(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				trainStationMap->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));

				assets.push_back(trainStationMap);
			}

			if (className == "DoubleStreetLampPost")
			{
				DoubleStreetLampPost* doubleStreetLampPost = new DoubleStreetLampPost(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				doubleStreetLampPost->SetModel(mModList->FindModel("DoubleStreetLampPost"));
				doubleStreetLampPost->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(doubleStreetLampPost);
			}

			if (className == "DoubleStreetLampPostWithMegaphone")
			{
				DoubleStreetLampPostWithMegaphone* doubleStreetLampPostWithMegaphone = new DoubleStreetLampPostWithMegaphone(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				doubleStreetLampPostWithMegaphone->SetModel(mModList->FindModel("DoubleStreetLampPostWithMegaphone"));
				doubleStreetLampPostWithMegaphone->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(doubleStreetLampPostWithMegaphone);
			}

			if (className == "FarbaMan")
			{
				FarbaMan* farbaMan = new FarbaMan(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				farbaMan->SetModel(mModList->FindModel("FarbaMan"));
				farbaMan->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				triggerableObjects.push_back(farbaMan);
				assets.push_back(farbaMan);
			}

			if (className == "Fence")
			{
				Fence* fence = new Fence(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				fence->SetModel(mModList->FindModel("Fence"));
				fence->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(fence);
			}

			if (className == "GrassGround")
			{
				GrassGround* grassGround = new GrassGround(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				grassGround->SetModel(mModList->FindModel("GrassGround"));
				grassGround->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(grassGround);
			}

			if (className == "GrassGroundNew")
			{
				GrassGroundNew* grassGroundNew = new GrassGroundNew(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				grassGroundNew->SetModel(mModList->FindModel("GrassGroundNew"));
				grassGroundNew->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(grassGroundNew);
			}

			if (className == "Soldier")
			{
				GreenSoldier* greenSoldier = new GreenSoldier(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				greenSoldier->SetModel(mModList->FindModel("Soldier"));
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

				information->SetModel(mModList->FindModel("Information"));
				information->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(information);
			}

			if (className == "InformationBoard")
			{
				InformationBoard* informationBoard = new InformationBoard(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				informationBoard->SetModel(mModList->FindModel("InformationBoard"));
				informationBoard->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(informationBoard);
			}

			if (className == "LongBuilding")
			{
				LongBuilding* longBuilding = new LongBuilding(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				longBuilding->SetModel(mModList->FindModel("LongBuilding"));
				longBuilding->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(longBuilding);
			}

			if (className == "OldBuilding")
			{
				OldBuilding* oldBuilding = new OldBuilding(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				oldBuilding->SetModel(mModList->FindModel("OldBuilding"));
				oldBuilding->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(oldBuilding);
			}

			if (className == "OverheadLines")
			{
				OverheadLines* overheadLines = new OverheadLines(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				overheadLines->SetModel(mModList->FindModel("OverheadLines"));
				overheadLines->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(overheadLines);
			}

			if (className == "PassengerTrain")
			{
				PassengerTrain* passengerTrain = new PassengerTrain(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				passengerTrain->SetModel(mModList->FindModel("PassengerTrain"));
				passengerTrain->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(passengerTrain);
			}

			if (className == "Pathcircles")
			{
				Pathcircles* pathcircles = new Pathcircles(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				pathcircles->SetModel(mModList->FindModel("Pathcircles"));
				pathcircles->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(pathcircles);
			}

			if (className == "PlatformGround1")
			{
				PlatformGround1* platformGround1 = new PlatformGround1(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				platformGround1->SetModel(mModList->FindModel("PlatformGround1"));
				platformGround1->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(platformGround1);
			}

			if (className == "PlatformGround2")
			{
				PlatformGround2* platformGround2 = new PlatformGround2(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				platformGround2->SetModel(mModList->FindModel("PlatformGround2"));
				platformGround2->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(platformGround2);
			}

			if (className == "PlatformGroundPlain")
			{
				PlatformGroundPlain* platformGroundPlain = new PlatformGroundPlain(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				platformGroundPlain->SetModel(mModList->FindModel("PlatformGroundPlain"));
				platformGroundPlain->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(platformGroundPlain);
			}

			if (className == "PlatformNumber1")
			{
				PlatformNumber1* platformNumber1 = new PlatformNumber1(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				platformNumber1->SetModel(mModList->FindModel("PlatformNumber1"));
				platformNumber1->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(platformNumber1);
			}

			if (className == "PlatformNumber2And3")
			{
				PlatformNumber2And3* platformNumber2And3 = new PlatformNumber2And3(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				platformNumber2And3->SetModel(mModList->FindModel("PlatformNumber2And3"));
				platformNumber2And3->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(platformNumber2And3);
			}

			if (className == "PlatformNumber4")
			{
				PlatformNumber4* platformNumber4 = new PlatformNumber4(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				platformNumber4->SetModel(mModList->FindModel("PlatformNumber4"));
				platformNumber4->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(platformNumber4);
			}

			if (className == "PoliceCar")
			{
				PoliceCar* policeCar = new PoliceCar(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				policeCar->SetModel(mModList->FindModel("PoliceCar"));
				policeCar->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(policeCar);
			}

			if (className == "Policeman")
			{
				Policeman* policeman = new Policeman(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				policeman->SetModel(mModList->FindModel("Policeman"));
				policeman->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				triggerableObjects.push_back(policeman);
				assets.push_back(policeman);
			}

			if (className == "PoliceStation")
			{
				PoliceStation* policeStation = new PoliceStation(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				policeStation->SetModel(mModList->FindModel("PoliceStation"));
				policeStation->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(policeStation);
			}

			if (className == "SimpleBuilding")
			{
				SimpleBuilding* simpleBuilding = new SimpleBuilding(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				simpleBuilding->SetModel(mModList->FindModel("SimpleBuilding"));
				simpleBuilding->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(simpleBuilding);
			}

			if (className == "SingleStreetLampPost")
			{
				SingleStreetLampPost* singleStreetLampPost = new SingleStreetLampPost(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				singleStreetLampPost->SetModel(mModList->FindModel("SingleStreetLampPost"));
				singleStreetLampPost->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(singleStreetLampPost);
			}

			if (className == "StoneGround")
			{
				StoneGround* stoneGround = new StoneGround(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				stoneGround->SetModel(mModList->FindModel("StoneGround"));
				stoneGround->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(stoneGround);
			}

			if (className == "Track")
			{
				Track* track = new Track(game, camera,
					gameObjects.at(i).GetPosition(),
					gameObjects.at(i).GetRotation(),
					gameObjects.at(i).GetScale());

				track->SetModel(mModList->FindModel("Track"));
				track->SetNode(NodeList::MovedToNode(gameObjects.at(i).GetPosition(), listNode));
				assets.push_back(track);
			}
		}

		return assets;
	}

	GameObject* GameObjectPusher::GetGameObjectByName(Game& game, Camera& camera, std::string className)
	{
		if (className == "Barrier")
		{
			Barrier* barrier = new Barrier(game, camera);
			return barrier;
		}

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

		if (className == "TrainStationMap")
		{
			TrainStationMap* trainStationMap = new TrainStationMap(game, camera);
			return trainStationMap;
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

		if (className == "Fence")
		{
			Fence* fence = new Fence(game, camera);
			return fence;
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

		if (className == "LongBuilding")
		{
			LongBuilding* longBuilding = new LongBuilding(game, camera);
			return longBuilding;
		}

		if (className == "OldBuilding")
		{
			OldBuilding* oldBuilding = new OldBuilding(game, camera);
			return oldBuilding;
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

		if (className == "PlatformGroundPlain")
		{
			PlatformGroundPlain* platformGroundPlain = new PlatformGroundPlain(game, camera);
			return platformGroundPlain;
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

		if (className == "PoliceStation")
		{
			PoliceStation* policeStation = new PoliceStation(game, camera);
			return policeStation;
		}

		if (className == "SimpleBuilding")
		{
			SimpleBuilding* simpleBuilding = new SimpleBuilding(game, camera);
			return simpleBuilding;
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