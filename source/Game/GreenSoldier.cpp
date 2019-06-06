#include "GreenSoldier.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "AssetList.h"

namespace Rendering
{
	GreenSoldier::GreenSoldier(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: AnimatedGameObject(game, camera, 
			"Soldier",
			startPosition,
			startRotation,
			startScale)
	{
		mRotationSpeed = 1;
		mTranslationSpeed = 0.05;

		mIsSelectedDiffuseMap = "Content\\Textures\\SoldierSelectedDiffuseMap.jpg";
		mIsBusyDiffuseMap = "Content\\Textures\\SoldierBusyDiffuseMap.jpg";

		SetAnimations();

		mSpotLight = new SpotLight(game);
		mPointLight = new PointLight(game);
	}


	GreenSoldier::~GreenSoldier()
	{
		DeleteObject(mSpotLight);
		DeleteObject(mPointLight);
	}

	void GreenSoldier::Initialize()
	{
		AnimatedGameObject::Initialize();
		AnimatedGameObject::BuildBoundingBox(XMFLOAT3(3.f, 12.f, 3.f));
		this->mCollider->setTriggerReaction(PLAYER_UNIT, mPosition, { 15.f, 12.f, 15.f });

		mPointLight->SetRadius(30.0f);
		mPointLight->SetPosition(mPosition);
		mPointLight->SetColor(Colors::White - SimpleMath::Vector3(0.0f, 0.0f, 0.5f));
	}

	void GreenSoldier::Update(const GameTime& gameTime)
	{
		AnimatedGameObject::Update(gameTime);
		XMFLOAT3 pointLightPosition = XMFLOAT3(mPosition.x, mPosition.y + 5, mPosition.z - 10);

		mPointLight->SetPosition(pointLightPosition);
		mSpotLight->SetPosition(mPosition);
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

	PointLight* GreenSoldier::GetPointLight()
	{
		return mPointLight;
	}

	SpotLight* GreenSoldier::GetSpotLight()
	{
		return mSpotLight;
	}
}


