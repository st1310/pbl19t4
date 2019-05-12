#include "PassengerTrain.h"

namespace Rendering
{
	PassengerTrain::PassengerTrain(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera, 
			"PassengerTrain",
			"Content\\Models\\PassengerTrain.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\PassengerTrainDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale)
	{
	}


	PassengerTrain::~PassengerTrain()
	{
	}
}