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
}