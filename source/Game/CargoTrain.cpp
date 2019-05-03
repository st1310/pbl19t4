#include "CargoTrain.h"

namespace Rendering
{
	CargoTrain::CargoTrain(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: GameObject(game, camera, CARGO_TRAIN,
			"Content\\Models\\CargoTrain.fbx",
			L"Content\\Effects\\SkinnedModel.cso",
			startPosition,
			startRotation,
			startScale)
	{
	}


	CargoTrain::~CargoTrain()
	{
	}
}