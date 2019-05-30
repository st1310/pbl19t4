#include "GrassGround.h"

namespace Rendering
{
	GrassGround::GrassGround(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"GrassGround",
			L"Content\\Effects\\TextureMapping.cso",
			startPosition,
			startRotation,
			startScale)
	{
	}


	GrassGround::~GrassGround()
	{
	}
}
