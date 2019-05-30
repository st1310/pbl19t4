#include "FarbaMan.h"

namespace Rendering
{
	FarbaMan::FarbaMan(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: AnimatedGameObject(game, camera,
			"FarbaMan",
			"Content\\Models\\FarbaMan.fbx",
			L"Content\\Effects\\SkinnedModel.cso",
			"Content\\Textures\\FarbaManDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale)
	{
		mRotationSpeed = 1;
		mTranslationSpeed = 0.05;

		mIsSelectedDiffuseMap = "Content\\Textures\\SoldierSelectedDiffuseMap.jpg";
		mIsBusyDiffuseMap = "Content\\Textures\\SoldierBusyDiffuseMap.jpg";

		SetAnimations();
	}


	FarbaMan::~FarbaMan()
	{

	}

	void FarbaMan::Initialize()
	{
		AnimatedGameObject::Initialize();
		AnimatedGameObject::BuildBoundingBox(XMFLOAT3(3.25f, 10.5f, 3.25f));
	}

	void FarbaMan::setSelection(bool selection)
	{
		isSelected = selection;

		if (isSelected)
			ChangeTexture(mIsSelectedDiffuseMap);

		else if (!isSelected && !mIsBusy)
			ChangeTexture(mDiffuseMap);

		else if (!isSelected && mIsBusy)
			ChangeTexture(mIsBusyDiffuseMap);
	}

	bool FarbaMan::getIsSelected()
	{
		return isSelected;
	}

	void FarbaMan::CheckTriggers()
	{
		std::vector<TypesTriggerReactions> helper;
		helper = mNode->trippedTriggers(this->getPosition());

		if (helper.empty())
			return;

		for (TypesTriggerReactions chck : helper)
		{
			switch (chck)
			{
			case Library::POLICE_CATCHING:
				//Remove from corresponding tables - GameObjects and listOfSoldiers
				this->~FarbaMan();
				break;
			case Library::PAINT:
				//Remove paint if right unit
				break;
			case Library::PAINTING_POSITION:
				//Allow painting if right unit
				break;
			default:
				break;
			}
		}
	}

	void FarbaMan::SetAnimations()
	{
		mAnimations.insert(std::pair<std::string, int>("Idle", 0));
		mAnimations.insert(std::pair<std::string, int>("StartRunning", 1));
		mAnimations.insert(std::pair<std::string, int>("Run", 2));
		mAnimations.insert(std::pair<std::string, int>("StopRunning", 3));
		mAnimations.insert(std::pair<std::string, int>("Reload", 4));
		mAnimations.insert(std::pair<std::string, int>("Paint", 5));
	}
}
