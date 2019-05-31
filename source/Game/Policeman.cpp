#include "Policeman.h"

namespace Rendering
{
	Policeman::Policeman(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: AnimatedGameObject(game, camera,
			"Policeman",
			L"Content\\Effects\\SkinnedModel.cso",
			startPosition,
			startRotation,
			startScale)
	{
		SetAnimations();
	}


	Policeman::~Policeman()
	{
	}

	void Policeman::Initialize()
	{
		AnimatedGameObject::Initialize();
		AnimatedGameObject::BuildBoundingBox(XMFLOAT3(3.25f, 10.5f, 3.25f));
	}

	void Policeman::CheckTriggers()
	{
		std::vector<TypesTriggerReactions> helper;
		helper = mNode->trippedTriggers(this->getPosition());

		if (helper.empty())
			return;

		policeNearby = 0;
		for (TypesTriggerReactions chck : helper)
		{
			switch (chck)
			{
			case Library::POLICE_ALLIES:
				policeNearby++;
				break;
			case Library::POLICE_STATION:
				//If triggered - activate it
				break;
			case Library::PLAYER_UNIT:
				playerNearby++;
				//If policeNearby > playerNearby - provoke to attack player?
				break;
			case Library::PAINT:
				//Follow the path of paint
				break;
			default:
				break;
			}
		}
	}

	void Policeman::SetAnimations()
	{
		mAnimations.insert(std::pair<std::string, int>("Idle", 0));
		mAnimations.insert(std::pair<std::string, int>("StartRunning", 1));
		mAnimations.insert(std::pair<std::string, int>("Run", 2));
		mAnimations.insert(std::pair<std::string, int>("StopRunning", 3));
		mAnimations.insert(std::pair<std::string, int>("StartAttack", 4));
		mAnimations.insert(std::pair<std::string, int>("Attack", 5));
		mAnimations.insert(std::pair<std::string, int>("StopAttack", 6));
		mAnimations.insert(std::pair<std::string, int>("Patrol", 7));
	}
}