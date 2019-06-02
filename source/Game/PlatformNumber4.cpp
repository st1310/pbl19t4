#include "PlatformNumber4.h"

namespace Rendering
{
	PlatformNumber4::PlatformNumber4(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"PlatformNumber4",
			startPosition,
			startRotation,
			startScale, true)
	{
	}


	PlatformNumber4::~PlatformNumber4()
	{
	}

	void PlatformNumber4::Initialize()
	{
		StaticGameObject::Initialize();
		StaticGameObject::BuildBoundingBox(XMFLOAT3(6.3f, 8.f, 9.f));
	}
}