#include "SingleStreetLampPost.h"

namespace Rendering
{
	SingleStreetLampPost::SingleStreetLampPost(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"SingleStreetLampPost",
			L"Content\\Effects\\TextureMapping.cso",
			startPosition,
			startRotation,
			startScale, true)
	{
	}


	SingleStreetLampPost::~SingleStreetLampPost()
	{
	}

	void SingleStreetLampPost::Initialize()
	{
		StaticGameObject::Initialize();
		StaticGameObject::BuildBoundingBox(XMFLOAT3(2.f, 15.f, 2.f));
	}
}