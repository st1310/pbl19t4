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
		mRotationSpeed = 3;
		mTranslationSpeed = 0.15;

		mIsSelectedDiffuseMap = "Content\\Textures\\SoldierSelectedDiffuseMap.jpg";
		mIsBusyDiffuseMap = "Content\\Textures\\SoldierBusyDiffuseMap.jpg";

		mPointLight = new PointLight(game);
		mPointLight->SetColor(Colors::Green - SimpleMath::Vector3(0.0f, 0.0f, 0.1f));
		mPointLight->SetRadius(30.0f);

		SetAnimations();
	}


	GreenSoldier::~GreenSoldier()
	{	
	}

	void GreenSoldier::Initialize()
	{
		AnimatedGameObject::Initialize();
		AnimatedGameObject::BuildSphere(4.5f);
		this->mCollider->setTriggerReaction(PLAYER_UNIT, mPosition, { 22.f, 12.f, 22.f });
	}

	void GreenSoldier::Update(const GameTime& gameTime)
	{
		AnimatedGameObject::Update(gameTime);

		XMFLOAT3 pointLightPosition = XMFLOAT3(mPosition.x + 10, mPosition.y + 5, mPosition.z - 10);
		mPointLight->SetPosition(pointLightPosition);

		if (this->getPosition().x > -102.0f && this->getPosition().x <-44.0f &&  this->getPosition().z > -25.0f && this->getPosition().z<23.0f) {
			achiveFarbaManPoint = true;//zespawnuj farbamana!!!
		}
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
			case Library::PAINTING_POSITION:
			
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

	bool GreenSoldier::getArchiveFarbaManPoint() {
		return achiveFarbaManPoint;
	}


	bool GreenSoldier::GetfootprintsInAreaFlag() {
		return footprintsInAreaFlag;
	}

	void GreenSoldier::SetfootprintsInAreaFlag(bool value) {
		footprintsInAreaFlag = value;
	}
}


