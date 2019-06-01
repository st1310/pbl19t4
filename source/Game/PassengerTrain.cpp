#include "PassengerTrain.h"

namespace Rendering
{
	PassengerTrain::PassengerTrain(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera, 
			"PassengerTrain",
			L"Content\\Effects\\TextureMapping.cso",
			startPosition,
			startRotation,
			startScale, true)
	{
	}


	PassengerTrain::~PassengerTrain()
	{
	}

	void PassengerTrain::Initialize()
	{
		StaticGameObject::Initialize();
		StaticGameObject::BuildBoundingBox(XMFLOAT3(110.5f, 10.5f, 12.5f));
	}
}