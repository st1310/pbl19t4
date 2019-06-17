#include "Barrier.h"

namespace Rendering
{
	Barrier::Barrier(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"Barrier",
			startPosition,
			startRotation,
			startScale)
	{
	}


	Barrier::~Barrier()
	{
	}
}