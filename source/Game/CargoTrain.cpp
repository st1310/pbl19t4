#include "CargoTrain.h"

namespace Rendering
{
	CargoTrain::CargoTrain(Game& game, Camera& camera, XMFLOAT4 position)
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