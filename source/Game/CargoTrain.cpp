#include "CargoTrain.h"

namespace Rendering
{
	CargoTrain::CargoTrain(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera, 
			"CargoTrain",
			L"Content\\Effects\\TextureMapping.cso",
			startPosition,
			startRotation,
			startScale, true)
	{
	}


	CargoTrain::~CargoTrain()
	{
	}

	void CargoTrain::Initialize()
	{
		StaticGameObject::Initialize();
		StaticGameObject::BuildBoundingBox(XMFLOAT3(110.5f, 10.5f, 12.5f));
	}
}