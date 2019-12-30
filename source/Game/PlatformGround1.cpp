#include "PlatformGround1.h"

namespace Rendering
{
	PlatformGround1::PlatformGround1(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"PlatformGround1",
			"Content\\Models\\PlatformGround1.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\PlatformGround1DiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale)
	{
	}


	PlatformGround1::~PlatformGround1()
	{
	}
}