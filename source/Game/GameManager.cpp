#include "GameManager.h"


namespace Rendering
{
	RTTI_DEFINITIONS(GameManager)

	GameManager::GameManager(Game& game, Camera& camera)
	{
		this->game = &game;
		this->camera = &camera;
		Initialize();
		
		//currentScene = MENU_LEVEL;
		currentScene = CITY_LEVEL;
		StartScene(currentScene);
	}


	GameManager::~GameManager()
	{
	}

	void GameManager::Initialize()
	{
		std::vector<CollisionNode*> listOfNodes;
		listOfNodes.clear();
		//Will rewrite to match maps
		CollisionNode* collNode1 = new CollisionNode({ 100.f, -100.f, 0.f }, { 0.f, 100.f, 100.f });
		CollisionNode* collNode2 = new CollisionNode({ 100.f, -100.f, -100.f }, { 0.f, 100.f, 0.f });
		CollisionNode* collNode12 = new CollisionNode({ 100.f, -100.f, -100.f }, { 0.f, 100.f, 100.f });
		collNode1->SetParent(collNode12); collNode2->SetParent(collNode12);
		collNode12->AddNewChild(collNode1); collNode12->AddNewChild(collNode2);
		CollisionNode* collNode3 = new CollisionNode({ 0.f, -100.f, 0.f }, { -100.f, 100.f, 100.f });
		CollisionNode* collNode4 = new CollisionNode({ 0.f, -100.f, -100.f }, { -100.f, 100.f, 0.f });
		CollisionNode* collNode34 = new CollisionNode({ 0.f, -100.f, -100.f }, { -100.f, 100.f, 100.f });
		CollisionNode* collNodeAll = new CollisionNode({ 100.f, -100.f, -100.f }, { -100.f, 100.f, 100.f });
		collNode3->SetParent(collNode34); collNode4->SetParent(collNode34);
		collNode34->AddNewChild(collNode3); collNode34->AddNewChild(collNode4);
		collNode12->SetParent(collNodeAll); collNode34->SetParent(collNodeAll);
		collNodeAll->AddNewChild(collNode34); collNodeAll->AddNewChild(collNode12);
		listOfNodes.push_back(collNodeAll);

		// Adding all scenes aka levels to Scenes
		MenuLevel* menuLevel = new MenuLevel(*game, *camera);
		Scenes.push_back(menuLevel);

		DayLevel* dayLevel = new DayLevel(*game, *camera);
		Scenes.push_back(dayLevel);

		TrainLevel* trainLevel = new TrainLevel(*game, *camera);
		Scenes.push_back(trainLevel);

		CityLevel* cityLevel = new CityLevel(*game, *camera);

		cityLevel->rewriteListOfNodes(listOfNodes);

		Scenes.push_back(cityLevel);


		CreationKitLevel* creationKitLevel = new CreationKitLevel(*game, *camera);
		
		Scenes.push_back(creationKitLevel);
	}

	void GameManager::StartScene(int sceneId)
	{
		// Clear old scene
		Scenes.at(currentScene)->Clear();

		currentScene = sceneId;

		// Fix scene Start 
		Scenes.at(sceneId)->Start(*game, *camera);	
	}

	int GameManager::GetSizeOfCurrentScene()
	{
		return Scenes.at(currentScene)->GameObjects.size();
	}

	std::vector<CollisionNode*> GameManager::GetCurrentListOfNodes()
	{
		return Scenes.at(currentScene)->getListOfNode();
	}

	void GameManager::SelectingUnits(XMVECTOR camOr, XMVECTOR ray, float dist, bool selectSeveral)
	{
		bool wasSelected = false;
		//For now - this is the prototype of checking if mouse clicked in right position
		for (GameComponent* gmCm : Scenes.at(currentScene)->GetUnitList())
		{
			GreenSoldier* greenSold = gmCm->As<GreenSoldier>();

			if (greenSold->getCollider()->CheckColliderIntersecteByRay(camOr, ray, dist) && (!wasSelected || selectSeveral))
			{
				greenSold->setSelection(true);
				//Will remove this later
				greenSold->SetVisible(false);
				wasSelected = true;
			}
			else if(!selectSeveral)
			{
				greenSold->setSelection(false);
				greenSold->SetVisible(true);
			}
				
		}


	}
}


