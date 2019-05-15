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
}