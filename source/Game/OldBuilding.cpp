#include "OldBuilding.h"

namespace Rendering
{
	OldBuilding::OldBuilding(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"OldBuilding",
			startPosition,
			startRotation,
			startScale, true)
	{
	}


	OldBuilding::~OldBuilding()
	{
	}

	void OldBuilding::Initialize()
	{
		StaticGameObject::Initialize();
		StaticGameObject::BuildBoundingBox(XMFLOAT3(6.3f, 8.f, 9.f));
	}
}