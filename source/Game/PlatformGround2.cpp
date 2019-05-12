#include "PlatformGround2.h"

namespace Rendering
{
	PlatformGround2::PlatformGround2(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"PlatformGround2",
			"Content\\Models\\PlatformGround2.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\PlatformGround2DiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale)
	{
	}


	PlatformGround2::~PlatformGround2()
	{
	}
}