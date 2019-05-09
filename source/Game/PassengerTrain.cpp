#include "PassengerTrain.h"

namespace Rendering
{
	PassengerTrain::PassengerTrain(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: GameObject(game, camera, "PassengerTrain",
			"Content\\Models\\PassengerTrain.fbx",
			L"Content\\Effects\\SkinnedModel.cso",
			startPosition,
			startRotation,
			startScale)
	{
	}


	PassengerTrain::~PassengerTrain()
	{
	}
}