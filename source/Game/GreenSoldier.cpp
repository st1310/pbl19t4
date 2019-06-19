#include "GreenSoldier.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "AssetList.h"
#include "GameCamera.h"

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
		mRotationSpeed = 6;
		mTranslationSpeed = 0.7;

		mIsSelectedDiffuseMap = "Content\\Textures\\SoldierSelectedDiffuseMap.jpg";
		mIsBusyDiffuseMap = "Content\\Textures\\SoldierBusyDiffuseMap.jpg";

		mSpotLight = new SpotLight(game);
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
		AnimatedGameObject::BuildBoundingBox(XMFLOAT3(3.f, 12.f, 3.f));
		this->mCollider->setTriggerReaction(PLAYER_UNIT, mPosition, { 15.f, 12.f, 15.f });
	}

	void GreenSoldier::Update(const GameTime& gameTime)
	{
		AnimatedGameObject::Update(gameTime);
		XMFLOAT3 pointLightPosition = XMFLOAT3(mPosition.x , mPosition.y + 5, mPosition.z - 10);

		mPointLight->SetPosition(pointLightPosition);
		mSpotLight->SetPosition(mPosition);

		if (this->getPosition().x > -102.0f && this->getPosition().x <-44.0f &&  
			this->getPosition().z > -25.0f && this->getPosition().z<23.0f) 
		{
			achiveFarbaManPoint = true;//zespawnuj farbamana!!!
		}
	}

	void GreenSoldier::setSelection(bool selection)
	{
		mIsSelected = selection;

		if (mIsSelected)
		{
			ChangeTexture(mIsSelectedDiffuseMap);
			mPointLight->SetColor(Colors::Yellow - SimpleMath::Vector3(0.0f, 0.0f, 0.1f));
		}
			

		else if (!mIsSelected && !mIsBusy)
		{
			std::string modelName = "Content\\Textures\\" + (std::string)mClassName + "DiffuseMap.jpg";
			ChangeTexture(modelName);
			mPointLight->SetColor(Colors::Green - SimpleMath::Vector3(0.0f, 0.0f, 0.1f));
		}

		else if (!mIsSelected && mIsBusy)
		{
			ChangeTexture(mIsBusyDiffuseMap);
			mPointLight->SetColor(Colors::Red - SimpleMath::Vector3(0.0f, 0.0f, 0.1f));
		}
			
	}

	bool GreenSoldier::getIsSelected()
	{
		return mIsSelected;
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

	SpotLight* GreenSoldier::GetSpotLight()
	{
		return mSpotLight;
	}

	bool GreenSoldier::getArchiveFarbaManPoint() {
		return achiveFarbaManPoint;
	}
}


