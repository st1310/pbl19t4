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
		//pathfinding->Solve_AStar();

		mChooseEndNode = true;
		GameObjectPusher pusher = GameObjectPusher();
		std::vector<GameComponent*> gameObjects = pusher.CreateAssets(game, camera, LoadFromFile());

		for (int i = 0; i < gameObjects.size(); i++)
		{
			this->GameObjects.push_back(gameObjects.at(i));
			GameObjects.at(i)->Initialize();
		}

		
		
	}

	void PathFinder_Test::Update(const GameTime& gameTime) {
		


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
			GameObjects.push_back(asset6);
			asset6->Initialize();
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_K)) {
			pathfinding->Solve_AStar();
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_J)) {
			if (pathfinding->currentNode->parent != nullptr) {
				pathfinding->currentNode = pathfinding->currentNode->parent;
				GameObject* asset3 = gameObjectPusher->GetGameObjectByName(*mGame, *mCamera, "GreenSoldier");
				asset3->mIsEdited = false;
				asset3->Translate(pathfinding->currentNode->x, 0, pathfinding->currentNode->y);
				GameObjects.push_back(asset3);
				asset3->Initialize();
				pathfinding->currentNode = pathfinding->currentNode->parent;
			}
			
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW) && mChooseEndNode) {
			if(indexNode<90)
				indexNode += 10;
		}
			

			if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW) && mChooseEndNode) {
				if(indexNode>9)
					indexNode -= 10;
		}
			

			if (mKeyboard->WasKeyPressedThisFrame(DIK_RIGHTARROW) && mChooseEndNode) {
				if(indexNode<99)
					indexNode++;
		}

			if (mKeyboard->WasKeyPressedThisFrame(DIK_LEFTARROW) && mChooseEndNode) {
				if(indexNode>0)
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