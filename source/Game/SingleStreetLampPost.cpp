#include "SingleStreetLampPost.h"

namespace Rendering
{
	SingleStreetLampPost::SingleStreetLampPost(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"SingleStreetLampPost",
			"Content\\Models\\SingleStreetLampPost.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\SingleStreetLampPostDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale, true)
	{
	}


	SingleStreetLampPost::~SingleStreetLampPost()
	{
	}
}