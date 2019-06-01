#include "GrassGroundNew.h"

namespace Rendering
{
	GrassGroundNew::GrassGroundNew(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"GrassGroundNew",
			L"Content\\Effects\\TextureMapping.cso",
			startPosition,
			startRotation,
			startScale)
	{
	}


	GrassGroundNew::~GrassGroundNew()
	{
	}
}