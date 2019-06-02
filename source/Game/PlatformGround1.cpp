#include "PlatformGround1.h"

namespace Rendering
{
	PlatformGround1::PlatformGround1(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"PlatformGround1",
			startPosition,
			startRotation,
			startScale)
	{
	}


	PlatformGround1::~PlatformGround1()
	{
	}
}