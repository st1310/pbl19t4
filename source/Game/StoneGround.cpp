#include "StoneGround.h"

namespace Rendering
{
	StoneGround::StoneGround(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"StoneGround",
			"Content\\Models\\StoneGround.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\StoneGroundDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale)
	{
	}


	StoneGround::~StoneGround()
	{
	}
}