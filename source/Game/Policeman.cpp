#include "Policeman.h"

namespace Rendering
{
	Policeman::Policeman(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: AnimatedGameObject(game, camera,
			"Policeman",
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
		AnimatedGameObject::BuildBoundingBox(XMFLOAT3(3.f, 12.f, 3.f));
		this->mCollider->setTriggerReaction(POLICE_CATCHING, mPosition, { 8.f, 12.f, 8.f });
		this->mCollider->setTriggerReaction(POLICE_ALLIES, mPosition, { 15.f, 12.f, 15.f });
	}

	void Policeman::CheckTriggers()
	{
		std::vector<TypesTriggerReactions> helper;
		if (inNode != nullptr)
			helper = inNode->trippedTriggers(this->getPosition());

		if (helper.empty())
			return;

		//It sees it's own triggers, so consider it
		policeNearby = -1;
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