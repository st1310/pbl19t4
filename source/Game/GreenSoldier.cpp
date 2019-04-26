#include "GreenSoldier.h"

namespace Rendering
{
	GreenSoldier::GreenSoldier(Game& game, Camera& camera, XMFLOAT3 position)
		: GameObject(game, camera,
			"Content\\Models\\Jednostka_green_baked.fbx",
			L"Content\\Effects\\SkinnedModel.cso",
			position)
	{
		this->SetVisible(true);
	}


	GreenSoldier::~GreenSoldier()
	{
	}
}


