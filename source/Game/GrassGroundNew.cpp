#include "GrassGroundNew.h"

namespace Rendering
{
	GrassGroundNew::GrassGroundNew(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"GrassGroundNew",
			"Content\\Models\\GrassGroundNew.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\GrassGroundNewDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale)
	{
	}


	GrassGroundNew::~GrassGroundNew()
	{
	}
}