#include "GreenSoldier.h"

namespace Rendering
{
	GreenSoldier::GreenSoldier(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: AnimatedGameObject(game, camera, 
			"Soldier",
			L"Content\\Effects\\SkinnedModel.cso",
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


	GreenSoldier::~GreenSoldier()
	{
	
	}

	void GreenSoldier::Initialize()
	{
		AnimatedGameObject::Initialize();
		AnimatedGameObject::BuildBoundingBox(XMFLOAT3(3.f, 12.f, 3.f));
		this->mCollider->setTriggerReaction(PLAYER_UNIT, mPosition, { 15.f, 12.f, 15.f });
	}

	void GreenSoldier::setSelection(bool selection)
	{
		isSelected = selection;

		if(isSelected)
			ChangeTexture(mIsSelectedDiffuseMap);

		else if (!isSelected && !mIsBusy)
		{
			std::string modelName = "Content\\Textures\\" + (std::string)mClassName + "DiffuseMap.jpg";
			ChangeTexture(modelName);
		}

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
		if (inNode != nullptr) 
			helper = inNode->trippedTriggers(this->getPosition());

		if (helper.empty())
			return;

		for (TypesTriggerReactions chck : helper)
		{
			switch (chck)
			{
			case Library::POLICE_CATCHING:
				//Remove from corresponding tables - GameObjects and listOfSoldiers
				mustBeDestroyed = true;
				return;
				break;
			case Library::PAINT:
				//It'll be sent to Paint object 
				break;
			default:
				break;
			}
		}
	}

	void GreenSoldier::SetAnimations()
	{
		mAnimations.insert(std::pair<std::string, int>("Idle", 0));
		mAnimations.insert(std::pair<std::string, int>("StartRunning", 1));
		mAnimations.insert(std::pair<std::string, int>("Run", 2));
		mAnimations.insert(std::pair<std::string, int>("StopRunning", 3));
	}
}


