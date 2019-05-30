#include "PlatformGround2.h"

namespace Rendering
{
	PlatformGround2::PlatformGround2(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"PlatformGround2",
			L"Content\\Effects\\TextureMapping.cso",
			startPosition,
			startRotation,
			startScale)
	{
	}


	PlatformGround2::~PlatformGround2()
	{
	}
}