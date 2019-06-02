#include "LongBuilding.h"

namespace Rendering
{
	LongBuilding::LongBuilding(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"LongBuilding",
			startPosition,
			startRotation,
			startScale, true)
	{
	}


	LongBuilding::~LongBuilding()
	{
	}

	void LongBuilding::Initialize()
	{
		StaticGameObject::Initialize();
		StaticGameObject::BuildBoundingBox(XMFLOAT3(6.3f, 8.f, 9.f));
	}
}