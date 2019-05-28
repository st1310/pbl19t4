#include "FarbaMan.h"

namespace Rendering
{
	FarbaMan::FarbaMan(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: AnimatedGameObject(game, camera,
			"FarbaMan",
			"Content\\Models\\FarbaMan.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\FarbaManDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale)
	{
	}

	FarbaMan::~FarbaMan()
	{
	}
}