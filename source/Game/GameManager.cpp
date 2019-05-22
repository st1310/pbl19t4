#include "GameManager.h"


namespace Rendering
{
	RTTI_DEFINITIONS(GameManager)

	GameManager::GameManager(Game& game, Camera& camera)
	{
		this->game = &game;
		this->camera = &camera;
		Initialize();

		mCurrentScene = CITY_LEVEL;
		StartScene(mCurrentScene);
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
		mScenes.push_back(menuLevel);

		DayLevel* dayLevel = new DayLevel(*game, *camera);
		mScenes.push_back(dayLevel);

		TrainLevel* trainLevel = new TrainLevel(*game, *camera);
		mScenes.push_back(trainLevel);

		CityLevel* cityLevel= new CityLevel(*game, *camera);
		cityLevel->rewriteListOfNodes(listOfNodes);
		mScenes.push_back(cityLevel);

		CreationKitLevel* creationKitLevel = new CreationKitLevel(*game, *camera);
		mScenes.push_back(creationKitLevel);

		PathFinder_Test* pathFinder_Test = new PathFinder_Test(*game, *camera);
		mScenes.push_back(pathFinder_Test);
	}

	void GameManager::StartScene(int sceneId)
	{
		if (sceneId < 0 || sceneId >= mScenes.size())
			return;

		// Clear old scene
		mScenes.at(mCurrentScene)->Clear();

		mCurrentScene = sceneId;

		mScenes.at(sceneId)->Start(*game, *camera);	
	}

	int GameManager::GetSizeOfCurrentScene()
	{
		return mScenes.at(mCurrentScene)->GameObjects.size();
	}

	void GameManager::Update(const GameTime& gameTime)
	{
		mScenes[mCurrentScene]->Update(gameTime);

		for(int i =0; i <  GetSizeOfCurrentScene(); i++)
			mScenes[mCurrentScene]->GameObjects[i]->Update(gameTime);
	}

	void GameManager::Draw(const GameTime& gameTime)
	{		
		for (GameComponent* component : mScenes[mCurrentScene]->GameObjects)
		{
			DrawableGameComponent* drawableGameComponent = component->As<DrawableGameComponent>();
			if (drawableGameComponent != nullptr)
				drawableGameComponent->Draw(gameTime);
		}

		mScenes[mCurrentScene]->Draw(gameTime);
	}

	std::vector<CollisionNode*> GameManager::GetCurrentListOfNodes()
	{
		return mScenes.at(mCurrentScene)->getListOfNode();
	}

	void GameManager::SelectingGround(long mouseX, long mouseY) {
		FirstPersonCamera* frCam = camera->As<FirstPersonCamera>();
		XMMATRIX viewProj = frCam->ViewProjectionMatrix();
		XMMATRIX invProjectionView = DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(viewProj), viewProj);

		float x = (((2.0f * mouseX) / (float)game->ScreenWidth()) - 1.0f);
		float y = (((2.0f * mouseY) / (float)game->ScreenHeight()) - 1.0f) * (-1.0f);

		XMVECTOR farPoint = XMVECTOR({ x, y, 1.0f, 0.0f });
		XMVECTOR TrF = XMVector3Transform(farPoint, invProjectionView);
		TrF = XMVector3Normalize(TrF);

		bool wasSelected = false;
	}

	//void GameManager::SelectingUnits(XMVECTOR camOr, XMVECTOR ray, float dist, bool selectSeveral)
	void GameManager::SelectingUnits(long mouseX, long mouseY, bool selectSeveral)
	{
		FirstPersonCamera* frCam = camera->As<FirstPersonCamera>();

		XMMATRIX viewProj = frCam->ViewProjectionMatrix();
		XMMATRIX invProjectionView = DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(viewProj), viewProj);

		float x = (((2.0f * mouseX) / (float)game->ScreenWidth()) - 1.0f);
		float y = (((2.0f * mouseY) / (float)game->ScreenHeight()) - 1.0f) * (-1.0f);

		XMVECTOR farPoint = XMVECTOR({ x, y, 1.0f, 0.0f });
		XMVECTOR TrF = XMVector3Transform(farPoint, invProjectionView);
		TrF = XMVector3Normalize(TrF);

		bool wasSelected = false;
		//For now - this is the prototype of checking if mouse clicked in right position
		for (GameComponent* gmCm : mScenes.at(mCurrentScene)->GetUnitList())
		{
			GreenSoldier* greenSold = gmCm->As<GreenSoldier>();

			if (greenSold->getCollider()->CheckColliderIntersecteByRay(frCam->PositionVector(), TrF, frCam->FarPlaneDistance()) && (!wasSelected || selectSeveral))
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


