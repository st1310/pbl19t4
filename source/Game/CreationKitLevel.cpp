#include "CreationKitLevel.h"

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
			mCurrentAssetIndex += 5;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_LEFTARROW) && !mEditAsset)
			mCurrentAssetIndex -= 5;

		if (mCurrentAssetIndex < 0)
			mCurrentAssetIndex = 13 - 1;

		if (mCurrentAssetIndex >= 13)
			mCurrentAssetIndex = 0;
	}

	void CreationKitLevel::Draw(const GameTime& gameTime)
	{
		mRenderStateHelper.SaveAll();
		mSpriteBatch->Begin();

		std::wostringstream helpLabel;

		if (!mEditAsset)
		{
			helpLabel << "Index/Size: "<< mCurrentAssetIndex + 1 << "/" << 13;
			helpLabel << "\nName: " << mAssets[mCurrentAssetIndex].c_str();
		}		

		mSpriteFont->DrawString(mSpriteBatch, helpLabel.str().c_str(), mTextPosition);

		mSpriteBatch->End();
		mRenderStateHelper.RestoreAll();
	}

	void CreationKitLevel::Start(Game& game, Camera& camera)
	{
		Initialize();
		GameObjectPusher pusher = GameObjectPusher();

		std::vector<GameComponent*> gameObjects = pusher.CreateAssets(game, camera, LoadFromFile());

		for (int i = 0; i < gameObjects.size(); i++)
		{
			this->GameObjects.push_back(gameObjects.at(i));
			GameObjects.at(i)->Initialize();
		}
	}
}