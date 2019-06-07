#include "GameManager.h"
#include "NodeList.h"
#include "GUI.h"

namespace Rendering
{
	namespace Library {

	}
	class GUI;
	RTTI_DEFINITIONS(GameManager)

		GameManager::GameManager(Game& game, Camera& camera)
	{
		this->game = &game;
		this->camera = &camera;
		Initialize();

		mCurrentScene = MENU_LEVEL;
		StartScene(mCurrentScene);
		unitsReadyToMove = false;
		ShowMousePosition = false;
		targetPos = XMFLOAT3(0, 0, 0);
		
		
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
		guiButtons.clear();
		if (sceneId < 0 || sceneId >= mScenes.size())
			return;

		// Clear old scene
		mScenes.at(mCurrentScene)->Clear();

		pathfinding = new PathFinding();
		pathfinding->setMapWidth(27);
		pathfinding->setMapHeight(145);
		pathfinding->OnUserCreate();

		mCurrentScene = sceneId;

		mScenes.at(sceneId)->ResetLights();
		mScenes.at(sceneId)->Start(*game, *camera);
		mScenes.at(sceneId)->SetGroudndCollider(pathfinding->collider);

		game->ClearAndSetNodes(mScenes.at(sceneId)->getListOfNode());

		for (int i = 0; i < mScenes.at(mCurrentScene)->GetUnitList().size(); i++) {
			mScenes.at(mCurrentScene)->GetUnitList().at(i)->SetUnitID(i);
			guiButtons.push_back(new Button(*game, *camera, XMFLOAT2(0 + 50 * i, 660.0f), XMFLOAT2(120.f, 120.f), "cos"));
		}

		GameCamera* gmCam = camera->As<GameCamera>();
		gmCam->SceneLoad();


	}

	std::vector<DrawableGameComponent*> GameManager::GetListOfUnits() {
		return mScenes.at(mCurrentScene)->GetUnitList();
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

		XMMATRIX viewProj = camera->ViewProjectionMatrix();
		XMMATRIX invProjectionView = DirectX::XMMatrixInverse(&DirectX::XMMatrixDeterminant(viewProj), viewProj);

		float x = (((2.0f * mouseX) / (float)game->ScreenWidth()) - 1.0f);
		float y = (((2.0f * mouseY) / (float)game->ScreenHeight()) - 1.0f) * (-1.0f);

		XMVECTOR farPoint = XMVECTOR({ x, y, 1.0f, 0.0f });
		XMVECTOR TrF = XMVector3Transform(farPoint, invProjectionView);
		TrF = XMVector3Normalize(TrF);


		if (mScenes.at(mCurrentScene) != nullptr)
		for (BoundingBox* bbx: mScenes.at(mCurrentScene)->GetGroundCollider()->GetBoundingBox()) {
			float farPlaneDistance = camera->FarPlaneDistance();
			if (bbx->Intersects(camera->PositionVector(), TrF, farPlaneDistance)) {
				
				targetPos = bbx->Center;	
				
				int mapHeight = pathfinding->getMapheight();
				int mapWidth = pathfinding->getMapWidth();
				int pathFindingMapSize = mapHeight * mapWidth;

				for (int j = 0; j < pathFindingMapSize; j++) {
					if (pathfinding->nodes[j].x == targetPos.x && pathfinding->nodes[j].y == targetPos.z) {	
						
						for (int i = 0; i < mScenes.at(mCurrentScene)->GetUnitList().size(); i++) {

							AnimatedGameObject* gameObject = (AnimatedGameObject*)(mScenes.at(mCurrentScene)->GetUnitList().at(i));

							if (gameObject->getIsSelected()) {

								for (int x = 0; x < pathFindingMapSize; x++) {
									if (gameObject->getPosition().x == pathfinding->nodes[x].x && gameObject->getPosition().z == pathfinding->nodes[x].y) {
										pathfinding->nodeStart = &pathfinding->nodes[x];
										pathfinding->nodeEnd = &pathfinding->nodes[j];
										pathfinding->currentNode = pathfinding->nodeEnd;
										pathfinding->Solve_AStar();
										XMFLOAT3 unitPosition = gameObject->getPosition();
										std::vector<XMFLOAT2> nextPositions = std::vector<XMFLOAT2>();
										nextPositions = pathfinding->GetPathNodesPosVector();
										gameObject->StartMoving(nextPositions);
										gameObject->RunInit();

										gameObject->setIsSelected(false);
									}
								}

   							}
						}
					}
				}
			}
		}
	}

	void GameManager::SelectingUnits(float mouseX, float mouseY)
	{
		unitID = -1;

		XMMATRIX viewProj = camera->ViewProjectionMatrix();
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

			if (greenSold->getCollider()->CheckColliderIntersecteByRay(camera->PositionVector(), TrF, camera->FarPlaneDistance()) && (!wasSelected))
			{
				greenSold->setSelection(true);
				greenSold->setIsSelected(true);
				//Will remove this later
				wasSelected = true;
				unitsReadyToMove = true;
				unitID = greenSold->GetUnitID();
			}
			else
			{
				greenSold->setSelection(false);
				greenSold->setIsSelected(false);
				unitsReadyToMove = false;
			}
		}

	}

	void GameManager::SelectingUnits(float mouse1X, float mouse1Y, float mouse2X, float mouse2Y)
	{
		GameCamera* gmCam = camera->As<GameCamera>();

		XMMATRIX viewProj = gmCam->ViewProjectionMatrix();
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

		for (float i = x1; i <= x2; i += 0.1f)
		{
			for (float j = y1; j >= y2; j -= 0.1f)
			{
				XMVECTOR farPoint = XMVECTOR({ i, j, 1.0f, 0.0f });
				XMVECTOR TrF = XMVector3Transform(farPoint, invProjectionView);
				TrF = XMVector3Normalize(TrF);


				//For now - this is the prototype of checking if mouse clicked in right position
				for (GameComponent* gmCm : mScenes.at(mCurrentScene)->GetUnitList())
				{
					GreenSoldier* greenSold = gmCm->As<GreenSoldier>();

					if (greenSold->getCollider()->CheckColliderIntersecteByRay(gmCam->PositionVector(), TrF, gmCam->FarPlaneDistance()))
					{
						greenSold->setSelection(true);
						greenSold->setIsSelected(true);
						unitsReadyToMove = true;
					}
				}
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

	
}

	



