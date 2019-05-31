#include "PathFinder_Test.h"

namespace Rendering
{
	PathFinder_Test::PathFinder_Test(Game& game, Camera& camera)
		:Scene(game, camera, PATHFINDER_TEST, "Content\\Serializations\\test_level.data")
	{
		gameObjectPusher = new GameObjectPusher();
		pathfinding = new PathFinding();
		mChooseEndNode = false;
	}
	PathFinder_Test::~PathFinder_Test()
	{
	}

	void PathFinder_Test::Start(Game& game, Camera& camera)
	{
		Initialize();
		pathfinding->OnUserCreate();
		ClearUnitList();
		mChooseEndNode = true;
		GameObjectPusher pusher = GameObjectPusher();
		pusher.listNode = this->getListOfNode();
		std::vector<GameComponent*> gameObjects = pusher.CreateAssets(game, camera, LoadFromFile());

		RewriteUnitList(pusher.listOfSoldiers);
		trigerrableObjects = pusher.triggerableObjects;

		for (int i = 0; i < gameObjects.size(); i++)
		{
			this->GameObjects.push_back(gameObjects.at(i));
			GameObjects.at(i)->Initialize();
		}
		
		for (DrawableGameComponent* drwGm : trigerrableObjects)
		{
			GameObject* gmObj = drwGm->As<GameObject>();
			gmObj->getNode()->AddTriggerCollider(gmObj->getCollider());
		}

		this->groundCollider = pathfinding->collider;	//przypisanie do kolajdera sceny kolajdera pathfindingu zawierającego wszystkie boundingboxy podłoża

	}

	//if(mGa)

	void PathFinder_Test::Update(const GameTime& gameTime) {

		if (mKeyboard->WasKeyPressedThisFrame(DIK_Y))
		{
			pathfinding->currentNode = &pathfinding->nodes[indexNode];
			GameObject* asset11 = gameObjectPusher->GetGameObjectByName(*mGame, *mCamera, "Pathcircles");
			asset11->mIsEdited = false;
			asset11->Translate(pathfinding->currentNode->x, 0, pathfinding->currentNode->y);
			this->AddUnitToList(asset11);
			GameObjects.push_back(asset11);
			pathfinding->nodes[indexNode].bObstacle = 0;
			asset11->Initialize();
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_H))
		{
			//GameObjects.pop_back();
			pathfinding->nodeEnd = &pathfinding->nodes[indexNode];
			GameObject* asset2 = gameObjectPusher->GetGameObjectByName(*mGame, *mCamera, "GreenSoldier");
			asset2->mIsEdited = false;
			asset2->Translate(pathfinding->nodeEnd->x, 0, pathfinding->nodeEnd->y);
			GameObjects.push_back(asset2);
			asset2->Initialize();
			pathfinding->currentNode = pathfinding->nodeEnd;
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_G)) {

			pathfinding->nodeStart = &pathfinding->nodes[indexNode];
			GameObject* asset6 = gameObjectPusher->GetGameObjectByName(*mGame, *mCamera, "GreenSoldier");
			asset6->mIsEdited = false;
			asset6->Translate(pathfinding->nodeStart->x, 0, pathfinding->nodeStart->y);
			this->AddUnitToList(asset6);
			GameObjects.push_back(asset6);
			asset6->Initialize();
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_O)) {
			pathfinding->currentNode = &pathfinding->nodes[indexNode];
			GameObject* asset7 = gameObjectPusher->GetGameObjectByName(*mGame, *mCamera, "SingleStreetLampPost");
			asset7->mIsEdited = false;
			asset7->Translate(pathfinding->currentNode->x, 0, pathfinding->currentNode->y);
			this->AddUnitToList(asset7);
			GameObjects.push_back(asset7);
			pathfinding->nodes[indexNode].bObstacle = 1;
			asset7->Initialize();
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_K)) {
			pathfinding->currentNode = pathfinding->nodeEnd;
			pathfinding->Solve_AStar();
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_J)) {

			if (!(pathfinding->pathNodesPos.empty())) {
				GameObject* asset3 = gameObjectPusher->GetGameObjectByName(*mGame, *mCamera, "GreenSoldier");
				asset3->mIsEdited = false;
				XMFLOAT2 pos = pathfinding->pathNodesPos.back();
				asset3->Translate(pos.x, 0, pos.y);
				GameObjects.push_back(asset3);
				asset3->Initialize();
				pathfinding->pathNodesPos.pop_back();
			}

			/*if (pathfinding->currentNode->parent != nullptr) {
				pathfinding->currentNode = pathfinding->currentNode->parent;


				GameObject* asset3 = gameObjectPusher->GetGameObjectByName(*mGame, *mCamera, "GreenSoldier");
				asset3->mIsEdited = false;
				asset3->Translate(pathfinding->currentNode->x, 0, pathfinding->currentNode->y);
				GameObjects.push_back(asset3);
				asset3->Initialize();
				pathfinding->currentNode = pathfinding->currentNode->parent;
			}*/

		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW) && mChooseEndNode) {
			if (indexNode<90)
				indexNode += 10;
		}


		if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW) && mChooseEndNode) {
			if (indexNode>9)
				indexNode -= 10;
		}


		if (mKeyboard->WasKeyPressedThisFrame(DIK_RIGHTARROW) && mChooseEndNode) {
			if (indexNode<99)
				indexNode++;
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_LEFTARROW) && mChooseEndNode) {
			if (indexNode>0)
				indexNode--;
		}

	}

	void PathFinder_Test::Draw(const GameTime& gameTime) {
		mRenderStateHelper.SaveAll();
		mSpriteBatch->Begin();

		std::wostringstream helpLabel;

		if (mChooseEndNode)
		{
			helpLabel << "PATHFINDING";
			helpLabel << "\nChooseDestinationPoint: " << indexNode + 1;
		}

		mSpriteFont->DrawString(mSpriteBatch, helpLabel.str().c_str(), mTextPosition);

		mSpriteBatch->End();
		mRenderStateHelper.RestoreAll();
	}

}