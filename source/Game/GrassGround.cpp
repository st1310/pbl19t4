#include "GrassGround.h"

namespace Rendering
{
	GrassGround::GrassGround(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"GrassGround",
			"Content\\Models\\GrassGround.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\GrassGroundDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale)
	{
	}


	GrassGround::~GrassGround()
	{
	}
}
