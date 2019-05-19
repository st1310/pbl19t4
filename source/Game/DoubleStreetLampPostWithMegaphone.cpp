#include "DoubleStreetLampPostWithMegaphone.h"

namespace Rendering
{
	DoubleStreetLampPostWithMegaphone::DoubleStreetLampPostWithMegaphone(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"DoubleStreetLampPostWithMegaphone",
			"Content\\Models\\DoubleStreetLampPostWithMegaphone.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\DoubleStreetLampPostWithMegaphoneDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale, true)
	{
	}


	DoubleStreetLampPostWithMegaphone::~DoubleStreetLampPostWithMegaphone()
	{
	}

	void DoubleStreetLampPostWithMegaphone::Initialize()
	{
		StaticGameObject::Initialize();
		StaticGameObject::BuildBoundingBox(XMFLOAT3(2.f, 15.f, 2.f));
	}
}