#include "CargoTrain.h"

namespace Rendering
{
	CargoTrain::CargoTrain(Game& game, Camera& camera, XMFLOAT3 position)
		: GameObject(game, camera,
			"Content\\Models\\CargoTrain.fbx",
			L"Content\\Effects\\SkinnedModel.cso",
			position)
	{
	}


	CargoTrain::~CargoTrain()
	{
	}
}