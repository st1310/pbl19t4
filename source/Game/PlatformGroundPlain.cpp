#include "PlatformGroundPlain.h"

namespace Rendering
{
	PlatformGroundPlain::PlatformGroundPlain(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"PlatformGroundPlain",
			startPosition,
			startRotation,
			startScale)
	{
	}


	PlatformGroundPlain::~PlatformGroundPlain()
	{
	}
}