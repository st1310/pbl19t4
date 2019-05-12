#include "CargoTrain.h"

namespace Rendering
{
	CargoTrain::CargoTrain(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera, 
			"CargoTrain",
			"Content\\Models\\CargoTrain.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\CargoTrainDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale)
	{
	}


	CargoTrain::~CargoTrain()
	{
	}
}