#include "StoneGround.h"

namespace Rendering
{
	StoneGround::StoneGround(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"StoneGround",
			L"Content\\Effects\\TextureMapping.cso",
			startPosition,
			startRotation,
			startScale)
	{
	}


	StoneGround::~StoneGround()
	{
	}
}