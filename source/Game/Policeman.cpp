#include "Policeman.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "AssetList.h"

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
		mRotationSpeed = 6;
		mTranslationSpeed = 0.45;

		mSpotLight = new SpotLight(game);
		mPointLight = new PointLight(game);
		mPointLight->SetColor(Colors::Blue - SimpleMath::Vector3(0.0f, 0.0f, 0.2f));
		mPointLight->SetRadius(40.0f);
	}


	Policeman::~Policeman()
	{
		DeleteObject(mSpotLight);
		DeleteObject(mPointLight);
	}

	void Policeman::Initialize()
	{
		AnimatedGameObject::Initialize();
		AnimatedGameObject::BuildSphere(4.5f);
		this->mCollider->setTriggerReaction(POLICE_CATCHING, mPosition, { 8.f, 12.f, 8.f });
		this->mCollider->setTriggerReaction(POLICE_ALLIES, mPosition, { 15.f, 12.f, 15.f });
		//patrolPath.clear();
		//patrolPath.push_back(XMFLOAT2(10.0f, 10.0f));
	//	this->StartFollow();
	}

	void Policeman::Update(const GameTime& gameTime)
	{
		AnimatedGameObject::Update(gameTime);
		XMFLOAT3 pointLightPosition = XMFLOAT3(mPosition.x + 10, mPosition.y + 5, mPosition.z - 10);

		mPointLight->SetPosition(pointLightPosition);

		int time = (int)gameTime.TotalGameTime();
		time = time % 2;

		if (time == 1)
			mPointLight->SetColor(Colors::Blue - SimpleMath::Vector3(0.0f, 0.0f, 0.2f));

		else 
			mPointLight->SetColor(Colors::DarkRed - SimpleMath::Vector3(0.0f, 0.0f, 0.2f));

		mSpotLight->SetPosition(mPosition);
	}

	void Policeman::addPointToPatrolPath(XMFLOAT2 point) {
		patrolPath.push_back(point);
	}

	void Policeman::StartFollow() {
		if (!(patrolPath.empty())) {
			this->StartMoving(patrolPath);
		}
	}

	void Policeman::Patrol(std::vector<XMFLOAT2> patrolPoints)
	{
		if (patrolPoints.size() == 0)
			return;

		// Finding nearest point -> magic int but ciiii
		double minDistance = 40000;
		int nearestPointId = 0;

		std::vector<XMFLOAT2> verifiedPatrolPoints = std::vector<XMFLOAT2>();

		for (int i = 0; i < patrolPoints.size(); i++)
		{
			double distance = abs(mPosition.x - patrolPoints.at(i).x) + abs(mPosition.z - patrolPoints.at(i).y);

			if (distance < minDistance)
				nearestPointId = i;
		}

		// Nearest point must be first
		for (int i = nearestPointId; i < patrolPoints.size(); i++)
			verifiedPatrolPoints.push_back(patrolPoints.at(i));

		for (int i = 0; i < nearestPointId; i++)
			verifiedPatrolPoints.push_back(patrolPoints.at(i));

		StartMoving(verifiedPatrolPoints, true);
		this->RunInit();
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
		//mAnimations.insert(std::pair<std::string, int>("Run", 2));
		mAnimations.insert(std::pair<std::string, int>("Hehe", 2));
		mAnimations.insert(std::pair<std::string, int>("StopRunning", 3));
		mAnimations.insert(std::pair<std::string, int>("StartAttack", 4));
		mAnimations.insert(std::pair<std::string, int>("Attack", 5));
		mAnimations.insert(std::pair<std::string, int>("StopAttack", 6));
		//mAnimations.insert(std::pair<std::string, int>("Patrol", 7));
		mAnimations.insert(std::pair<std::string, int>("Run", 7));
	}

	PointLight* Policeman::GetPointLight()
	{
		return mPointLight;
	}

	SpotLight* Policeman::GetSpotLight()
	{
		return mSpotLight;
	}
}