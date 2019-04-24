#include "PassengerTrain.h"

namespace Rendering
{
	PassengerTrain::PassengerTrain(Game& game, Camera& camera, XMFLOAT3 position)
		: GameObject(game, camera,
			"Content\\Models\\PassengerTrain.fbx",
			L"Content\\Effects\\SkinnedModel.cso",
			position)
	{
	}


	PassengerTrain::~PassengerTrain()
	{
	}
}