#include "DoubleStreetLampPost.h"

namespace Rendering
{
	DoubleStreetLampPost::DoubleStreetLampPost(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"DoubleStreetLampPost",
			"Content\\Models\\DoubleStreetLampPost.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\DoubleStreetLampPostDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale, true)
	{
	}


	DoubleStreetLampPost::~DoubleStreetLampPost()
	{
	}
}