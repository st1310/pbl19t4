#include "GreenSoldier.h"

namespace Rendering
{
	GreenSoldier::GreenSoldier(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: AnimatedGameObject(game, camera, 
			"GreenSoldier",
			"Content\\Models\\Soldier.fbx",
			L"Content\\Effects\\SkinnedModel.cso",
			"Content\\Textures\\SoldierDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale)
	{
		mRotationSpeed = 1;
		mTranslationSpeed = 0.05;

		mIsSelectedDiffuseMap = "Content\\Textures\\SoldierSelectedDiffuseMap.jpg";
		mIsBusyDiffuseMap = "Content\\Textures\\SoldierBusyDiffuseMap.jpg";
	}


	GreenSoldier::~GreenSoldier()
	{
	
	}

	void GreenSoldier::Initialize()
	{
		AnimatedGameObject::Initialize();
		AnimatedGameObject::BuildBoundingBox(XMFLOAT3(3.25f, 10.5f, 3.25f));
	}

	void GreenSoldier::setSelection(bool selection)
	{
		isSelected = selection;

		if(isSelected)
			ChangeTexture(mIsSelectedDiffuseMap);

		else if(!isSelected && !mIsBusy)
			ChangeTexture(mDiffuseMap);

		else if (!isSelected && mIsBusy)
			ChangeTexture(mIsBusyDiffuseMap);
	}

	bool GreenSoldier::getIsSelected()
	{
		return isSelected;
	}


	void GreenSoldier::CheckTriggers()
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
				this->~GreenSoldier();
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
}


