#include "Policeman.h"

namespace Rendering
{
	Policeman::Policeman(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: AnimatedGameObject(game, camera,
			"Policeman",
			"Content\\Models\\Policeman.fbx",
			L"Content\\Effects\\SkinnedModel.cso",
			"Content\\Textures\\PolicemanDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale)
	{
		this->SetVisible(true);
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
		if (inNode != nullptr) 
			helper = inNode->trippedTriggers(this->getPosition());

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
}