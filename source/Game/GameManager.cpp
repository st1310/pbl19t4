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
		unitsReadyToMove = false;
		mouseX1 = 0;
		mouseY1 = 0;
		ShowMousePosition = false;
		targetPos = XMFLOAT3(0, 0, 0);
		targetSet = false;
		//pathfinding = new PathFinding();
	}

	GameManager::~GameManager()
	{
	}

	void GameManager::Initialize()
	{
		// Adding all scenes aka levels to Scenes
		MenuLevel* menuLevel = new MenuLevel(*game, *camera);
		mScenes.push_back(menuLevel);

		DayLevel* dayLevel = new DayLevel(*game, *camera);
		dayLevel->BuildNodesStart({ 100.f, -100.f, -50.f }, { -100.f, 100.f, 50.f });
		mScenes.push_back(dayLevel);

		TrainLevel* trainLevel = new TrainLevel(*game, *camera);
		trainLevel->BuildNodesStart({ 100.f, -100.f, -50.f }, { -100.f, 100.f, 50.f });
		mScenes.push_back(trainLevel);

		CityLevel* cityLevel = new CityLevel(*game, *camera);
		cityLevel->BuildNodesStart({ 200.f, -100.f, -250.f }, { -200.f, 100.f, 250.f });
		mScenes.push_back(cityLevel);

		CreationKitLevel* creationKitLevel = new CreationKitLevel(*game, *camera);
		//Need to find a way to get min/max coordinates from it... Dynamic check?
		creationKitLevel->BuildNodesStart({ 500.f, -100.f, -500.f }, { -500.f, 100.f, 500.f });
		mScenes.push_back(creationKitLevel);

		PathFinder_Test* pathFinder_Test = new PathFinder_Test(*game, *camera);
		pathFinder_Test->BuildNodesStart({ 200.f, -100.f, -200.f }, { -200.f, 100.f, 200.f });
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

		for (int i = 0; i < GetSizeOfCurrentScene(); i++)
			mScenes[mCurrentScene]->GameObjects[i]->Update(gameTime);
	}

	void GameManager::Draw(const GameTime& gameTime)
	{
		for (GameComponent* component : mScenes[mCurrentScene]->GameObjects)
		{
			DrawableGameComponent* drawableGameComponent = component->As<DrawableGameComponent>();
			if (drawableGameComponent != nullptr && drawableGameComponent->Visible())
				drawableGameComponent->Draw(gameTime);
		}

		mScenes[mCurrentScene]->Draw(gameTime);
	}

	std::vector<CollisionNode*> GameManager::GetCurrentListOfNodes()
	{
		return mScenes.at(mCurrentScene)->getListOfNode();
	}

	void GameManager::SelectingGrounds(long mouseX, long mouseY) {

		FirstPersonCamera* firstCam = camera->As<FirstPersonCamera>();

		XMMATRIX viewProj = firstCam->ViewProjectionMatrix();
		XMMATRIX invProjectionView = DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(viewProj), viewProj);

		float x = (((2.0f * mouseX) / (float)game->ScreenWidth()) - 1.0f);
		float y = (((2.0f * mouseY) / (float)game->ScreenHeight()) - 1.0f) * (-1.0f);

		XMVECTOR farPoint = XMVECTOR({ x, y, 1.0f, 0.0f });
		XMVECTOR TrF = XMVector3Transform(farPoint, invProjectionView);
		TrF = XMVector3Normalize(TrF);

		int boundingBoxId = 0;
		////mScenes.at(mCurrentScene)->SetGroudn

		for (BoundingBox* bbx: mScenes.at(mCurrentScene)->GetGroundCollider()->GetBoundingBox()) {
			float farPlaneDistance = firstCam->FarPlaneDistance();
			if (bbx->Intersects(firstCam->PositionVector(), TrF, farPlaneDistance)) {
				
				targetPos = bbx->Center;
				targetSet = true;
				//mScenes.at(5)->
				//PathFinder_Test* pathFinder_Test1 = new PathFinder_Test(*game, *camera);
				//mScenes.at(mCurrentScene)->GetGroundCollider()->GetBoundingBox().at()
				//pathFinder_Test1->SetEndNodePath(targetPos);
			}
			boundingBoxId++;
			//pathfinding
		}
	}

	void GameManager::SelectingUnits(long mouseX, long mouseY, bool selectSeveral)
	{
		FirstPersonCamera* firstCam = camera->As<FirstPersonCamera>();

		XMMATRIX viewProj = firstCam->ViewProjectionMatrix();
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

			if (greenSold->getCollider()->CheckColliderIntersecteByRay(firstCam->PositionVector(), TrF, firstCam->FarPlaneDistance()) && (!wasSelected))
			{
				greenSold->setSelection(true);
				//Will remove this later
				greenSold->SetVisible(false);
				wasSelected = true;
				unitsReadyToMove = true;
			}
			else if (greenSold->getCollider()->CheckColliderIntersecteByRay(firstCam->PositionVector(), TrF, firstCam->FarPlaneDistance()) && (selectSeveral)) {
				greenSold->setSelection(true);
				//Will remove this later
				greenSold->SetVisible(false);
				wasSelected = true;
				unitsReadyToMove = true;
			}

			else if (!selectSeveral)
			{
				greenSold->setSelection(false);
				greenSold->SetVisible(true);
				unitsReadyToMove = false;
			}

		}
	}

	int GameManager::GetCurrentSceneId() {
		return  mCurrentScene;
	}

	bool GameManager::GetunitsReadyToMove() {
		return unitsReadyToMove;
	}

	bool GameManager::GetShowMousePosition() {
		return ShowMousePosition;
	}


	float GameManager::GetMouseX() {
		return mouseX1;
	}

	float GameManager::GetMouseY() {
		return mouseY1;
	}

	XMFLOAT3 GameManager::GetTargetPos() {
		return targetPos;
	}
	
}

	



