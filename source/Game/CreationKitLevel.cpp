#include "CreationKitLevel.h"
#include "Camera.h"

namespace Rendering
{
	CreationKitLevel::CreationKitLevel(Game& game, Camera& camera)
		:Scene(game, camera, CREATION_KIT_LEVEL, "Content\\Serializations\\creation_kit_level.data")
	{
	}

	CreationKitLevel::~CreationKitLevel()
	{
	}

	void CreationKitLevel::Update(const GameTime& gameTime)
	{
		if (mKeyboard->WasKeyPressedThisFrame(DIK_N) && !mEditAsset)
		{
			GameObjectPusher* gameObjectPusher = new GameObjectPusher();
			std::string name = mAssets[mCurrentAssetIndex];
			GameObject* asset = gameObjectPusher->GetGameObjectByName(*mGame, *mCamera, name);

			asset->mIsEdited = true;
			GameObjects.push_back(asset);
			asset->Initialize();

			asset->mIsEdited = true;
			asset->mIsSelected = true;
			mEditAsset = true;
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_X) && mEditAsset)
			mEditAsset = false;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_M))
			SaveToFile();

		if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW) && !mEditAsset)
			mCurrentAssetIndex++;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW) && !mEditAsset)
			mCurrentAssetIndex--;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_RIGHTARROW) && !mEditAsset)
			mCurrentAssetIndex += mSpeedlyIterationFactor;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_LEFTARROW) && !mEditAsset)
			mCurrentAssetIndex -= mSpeedlyIterationFactor;

		if (mCurrentAssetIndex < 0)
			mCurrentAssetIndex = mAssetsSize - 1;

		if (mCurrentAssetIndex >= mAssetsSize)
			mCurrentAssetIndex = 0;
	}

	void CreationKitLevel::Draw(const GameTime& gameTime)
	{
		mRenderStateHelper.SaveAll();
		mSpriteBatch->Begin();

		std::wostringstream helpLabel;

		if (!mEditAsset)
		{
			helpLabel << "Index/Size: " << mCurrentAssetIndex + 1 << "/" << mAssetsSize;
			helpLabel << "\nName: " << mAssets[mCurrentAssetIndex].c_str();
		}		

		mSpriteFont->DrawString(mSpriteBatch, helpLabel.str().c_str(), mTextPosition);

		mSpriteBatch->End();
		mRenderStateHelper.RestoreAll();
	}

	void CreationKitLevel::Start(Game& game, Camera& camera)
	{
		Initialize();
		InitializeLights();
		GameObjectPusher pusher = GameObjectPusher();


		camera.SetPosition(0.0f, 70.0f, 20.0f);

		std::vector<GameComponent*> gameObjects = pusher.CreateAssets(game, camera, LoadFromFile(), true);

		RewriteUnitList(pusher.listOfSoldiers);
		trigerrableObjects = pusher.triggerableObjects;

		this->BuildNodesStart(pusher.GetPosA(), pusher.GetPosC());


		for (int i = 0; i < gameObjects.size(); i++)
		{
			this->GameObjects.push_back(gameObjects.at(i));
			GameObjects.at(i)->Initialize();

			GameObject* gmObj = GameObjects.at(i)->As<GameObject>();
			gmObj->SetNode(NodeList::MovedToNode(gmObj->getPosition(), getListOfNode()));

			StaticGameObject* stat = gmObj->As<StaticGameObject>();
			if (stat != nullptr)
			{
				gmObj->getNode()->AddStaticCollider(gmObj->getCollider());
			}
			else
			{
				AnimatedGameObject* anim = gmObj->As<AnimatedGameObject>();
				if (anim != nullptr)
				{
					gmObj->getNode()->AddDynamicCollider(gmObj->getCollider());
				}
			}
		}

		for (DrawableGameComponent* drwGm : trigerrableObjects)
		{
			GameObject* gmObj = drwGm->As<GameObject>();
			this->getListOfNode().at(0)->AddTriggerCollider(gmObj->getCollider());
		}

		// Add point Lights to soldiers
		for (int i = 0; i < gameObjects.size(); i++)
		{
			GameObject* gameObject = gameObjects.at(i)->As<GameObject>();
			std::string className = gameObject->GetName();

			if (className == "Soldier")
			{
				GreenSoldier* soldier = gameObjects.at(i)->As<GreenSoldier>();
				mPointLights.push_back(soldier->GetPointLight());
			}
		}

		// Add spot Lights to lamps
		for (int i = 0; i < gameObjects.size(); i++)
		{
			GameObject* gameObject = GameObjects.at(i)->As<GameObject>();
			std::string className = gameObject->GetName();

			if (className == "SingleStreetLampPost")
			{
				SingleStreetLampPost* singleStreetLampPost = gameObjects.at(i)->As<SingleStreetLampPost>();
				mSpotLights.push_back(singleStreetLampPost->GetSpotLight());
			}
		}
	}
}