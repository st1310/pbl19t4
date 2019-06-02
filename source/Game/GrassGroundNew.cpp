#include "GrassGroundNew.h"

namespace Rendering
{
	GrassGroundNew::GrassGroundNew(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"GrassGroundNew",
			startPosition,
			startRotation,
			startScale)
	{
	}


	GrassGroundNew::~GrassGroundNew()
	{
	}
}