#include "FarbaMan.h"
#include "PointLight.h"
#include "AssetList.h"

namespace Rendering
{
	FarbaMan::FarbaMan(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: AnimatedGameObject(game, camera,
			"FarbaMan",
			startPosition,
			startRotation,
			startScale)
	{
		mRotationSpeed = 6;
		mTranslationSpeed = 0.3;

		mIsSelectedDiffuseMap = "Content\\Textures\\SoldierSelectedDiffuseMap.jpg";
		mIsBusyDiffuseMap = "Content\\Textures\\SoldierBusyDiffuseMap.jpg";

		SetVisible(false);

		SetAnimations();

		mPointLight->SetColor(Colors::Purple - SimpleMath::Vector3(0.0f, 0.0f, 0.1f));
		mPointLight->SetRadius(0.0f);
	}


	FarbaMan::~FarbaMan()
	{

	}

	void FarbaMan::Initialize()
	{
		AnimatedGameObject::Initialize();
		AnimatedGameObject::BuildSphere(4.5f);
		this->mCollider->setTriggerReaction(PLAYER_UNIT, mPosition, { 9.f, 12.f, 9.f });
	}

	void FarbaMan::setSelection(bool selection)
	{
		isSelected = selection;

		if (isSelected)
		{
			ChangeTexture(mIsSelectedDiffuseMap);
			mPointLight->SetColor(Colors::Yellow - SimpleMath::Vector3(0.0f, 0.0f, 0.1f));
		}


		else if (!isSelected && !mIsBusy)
		{
			std::string modelName = "Content\\Textures\\" + (std::string)mClassName + "DiffuseMap.jpg";
			ChangeTexture(modelName);
			mPointLight->SetColor(Colors::Purple - SimpleMath::Vector3(0.0f, 0.0f, 0.1f));
		}

		else if (!isSelected && mIsBusy)
		{
			ChangeTexture(mIsBusyDiffuseMap);
			mPointLight->SetColor(Colors::Red - SimpleMath::Vector3(0.0f, 0.0f, 0.1f));
		}
	}

	void FarbaMan::Update(const GameTime& gameTime)
	{
		if (painting)
		{
			if (paintingTime == -1.f)
			{
				paintingTime = gameTime.TotalGameTime();
				AnimatedGameObject::ChangeAnimation("Paint");
				mIsBusy = true;
			}
			else if (gameTime.TotalGameTime() - paintingTime >= 25.f)
			{
				//You win, zog off
				painting = false;
				destroyPaintedPosition = true;
				AnimatedGameObject::ChangeAnimation("Idle");
				paintingTime = -1.f;
				mIsBusy = false;
			}
		}

		if (this->getPosition().x > 570.0f && this->getPosition().x <590.0f &&  this->getPosition().z > -230.0f && this->getPosition().z < -190.0f) {
			inPaintArea = true;
		}

		AnimatedGameObject::Update(gameTime);
		XMFLOAT3 pointLightPosition = XMFLOAT3(mPosition.x, mPosition.y + 5, mPosition.z - 10);

		mPointLight->SetPosition(pointLightPosition);
	}

	bool FarbaMan::getIsSelected()
	{
		return isSelected;
	}

	void FarbaMan::StartPainting()
	{
		painting = true;
	}

	void FarbaMan::CheckTriggers()
	{
		std::vector<TypesTriggerReactions> helper;
		if (inNode != nullptr)
			helper = inNode->trippedTriggers(this->getPosition());

		if (helper.empty())
			return;

		mAllowPainting = false;

		for (TypesTriggerReactions chck : helper)
		{
			switch (chck)
			{
			case Library::POLICE_CATCHING:
				//Remove from corresponding tables - GameObjects and listOfSoldiers
				mustBeDestroyed = true;
				break;
			case Library::PAINTING_POSITION:
				if (!painting)
					mAllowPainting = true;
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
