#include "DoubleStreetLampPost.h"

namespace Rendering
{
	DoubleStreetLampPost::DoubleStreetLampPost(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"DoubleStreetLampPost",
			startPosition,
			startRotation,
			startScale, true)
	{
	}


	DoubleStreetLampPost::~DoubleStreetLampPost()
	{
	}

	void DoubleStreetLampPost::Initialize()
	{
		StaticGameObject::Initialize();
		StaticGameObject::BuildBoundingBox(XMFLOAT3(2.f, 15.f, 2.f));
	}
}