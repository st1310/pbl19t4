#include "GameManager.h"
#include "NodeList.h"

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
		// Adding all scenes aka levels to Scenes
		MenuLevel* menuLevel = new MenuLevel(*game, *camera);
		mScenes.push_back(menuLevel);

		DayLevel* dayLevel = new DayLevel(*game, *camera);
		dayLevel->BuildNodesStart({ 100.f, -100.f, -50.f }, { -100.f, 100.f, 50.f });
		mScenes.push_back(dayLevel);

		TrainLevel* trainLevel = new TrainLevel(*game, *camera);
		trainLevel->BuildNodesStart({ 100.f, -100.f, -50.f }, { -100.f, 100.f, 50.f });
		mScenes.push_back(trainLevel);

		CityLevel* cityLevel= new CityLevel(*game, *camera);
		cityLevel->BuildNodesStart({200.f, -100.f, -250.f}, { -200.f, 100.f, 250.f });
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
		
		this->game->ClearAndSetNodes(mScenes.at(sceneId)->getListOfNode());
	}

	int GameManager::GetSizeOfCurrentScene()
	{
		return mScenes.at(mCurrentScene)->GameObjects.size();
	}

	void GameManager::Update(const GameTime& gameTime)
	{
		mScenes[mCurrentScene]->Update(gameTime);

		for (GameComponent* remCmp : mScenes[mCurrentScene]->GetTriggerableObjects())
		{
			GameObject* trgObj = remCmp->As<GameObject>();
			trgObj->CheckTriggers();

			if (trgObj->IsMustBeDestroyed())
			{
				auto itrScGO = std::find(mScenes[mCurrentScene]->GameObjects.begin(), mScenes[mCurrentScene]->GameObjects.end(), remCmp);

				if (itrScGO != mScenes[mCurrentScene]->GameObjects.end())
					mScenes[mCurrentScene]->GameObjects.erase(itrScGO);

				game->RemoveComponent(remCmp);
				//If it isn't soldier - it won't make exceptions
				mScenes[mCurrentScene]->RemoveUnitFromList(trgObj);
				mScenes[mCurrentScene]->RemoveTriggerableObjectFromList(trgObj);
				remCmp->~GameComponent();
			}
		}

		for(int i =0; i <  GetSizeOfCurrentScene(); i++)
			mScenes[mCurrentScene]->GameObjects[i]->Update(gameTime);
	}

	void GameManager::Draw(const GameTime& gameTime)
	{		
		for (GameComponent* component : mScenes[mCurrentScene]->GameObjects)
		{
			DrawableGameComponent* drawableGameComponent = component->As<DrawableGameComponent>();
			if (drawableGameComponent != nullptr && drawableGameComponent->Visible())
				if(drawableGameComponent->getNode() == nullptr)
					drawableGameComponent->Draw(gameTime);
				else if(NodeList::IsNodeInsideList(drawableGameComponent->getNode(), this->game->GetNodesInFructum()))
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

		//mScenes.at(mCurrentScene)->SetGroudn

		if (mScenes.at(mCurrentScene) != nullptr)
		for (BoundingBox* bbx: mScenes.at(mCurrentScene)->GetGroundCollider()->GetBoundingBox()) {
			float farPlaneDistance = firstCam->FarPlaneDistance();
			if (bbx->Intersects(firstCam->PositionVector(), TrF, farPlaneDistance)) {
				
				XMFLOAT3 targetPos = bbx->Center;
				
			}
		}
	}

	void GameManager::SelectingUnits(float mouseX, float mouseY)
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
				}
				else
				{
					greenSold->setSelection(false);
					greenSold->SetVisible(true);
				}

			}
	}

	void GameManager::SelectingUnits(float mouse1X, float mouse1Y, float mouse2X, float mouse2Y)
	{
		FirstPersonCamera* firstCam = camera->As<FirstPersonCamera>();

		XMMATRIX viewProj = firstCam->ViewProjectionMatrix();
		XMMATRIX invProjectionView = DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(viewProj), viewProj);

		if (mouse1X > mouse2X)
		{
			float tempMouse = mouse1X;
			mouse1X = mouse2X;
			mouse2X = tempMouse;
		}

		if (mouse1Y > mouse2Y)
		{
			float tempMouse = mouse1Y;
			mouse1Y = mouse2Y;
			mouse2Y = tempMouse;
		}

		float x1 = (((2.0f * mouse1X) / (float)game->ScreenWidth()) - 1.0f);
		float y1 = (((2.0f * mouse1Y) / (float)game->ScreenHeight()) - 1.0f) * (-1.0f);

		float x2 = (((2.0f * mouse2X) / (float)game->ScreenWidth()) - 1.0f);
		float y2 = (((2.0f * mouse2Y) / (float)game->ScreenHeight()) - 1.0f) * (-1.0f);

		for (float i = x1; i <= x2; i += 0.025f)
		{
			for (float j = y1; j >= y2; j -= 0.025f)
			{
				XMVECTOR farPoint = XMVECTOR({ i, j, 1.0f, 0.0f });
				XMVECTOR TrF = XMVector3Transform(farPoint, invProjectionView);
				TrF = XMVector3Normalize(TrF);


				//For now - this is the prototype of checking if mouse clicked in right position
				for (GameComponent* gmCm : mScenes.at(mCurrentScene)->GetUnitList())
				{
					GreenSoldier* greenSold = gmCm->As<GreenSoldier>();

					if (greenSold->getCollider()->CheckColliderIntersecteByRay(firstCam->PositionVector(), TrF, firstCam->FarPlaneDistance()))
					{
						greenSold->setSelection(true);
						greenSold->SetVisible(false);
					}

				}
			}
		}
	}
		
	int GameManager::GetCurrentSceneId() {
		return  mCurrentScene;
	}

	}

	



