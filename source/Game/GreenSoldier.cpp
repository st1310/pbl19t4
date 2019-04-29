#include "GreenSoldier.h"

namespace Rendering
{
	GreenSoldier::GreenSoldier(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: GameObject(game, camera,
			"Content\\Models\\Jednostka_green_baked.fbx",
			L"Content\\Effects\\SkinnedModel.cso",
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


