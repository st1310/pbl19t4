#include "GreenSoldier.h"

namespace Rendering
{
	GreenSoldier::GreenSoldier(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: AnimatedGameObject(game, camera, 
			"GreenSoldier",
			"Content\\Models\\GreenSoldier.fbx",
			L"Content\\Effects\\SkinnedModel.cso",
			"Content\\Textures\\GreenSoldierDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale)
	{
		this->SetVisible(true);
	}


	GreenSoldier::~GreenSoldier()
	{
	}
}


