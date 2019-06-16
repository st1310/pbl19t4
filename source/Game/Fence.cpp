#include "Fence.h"

namespace Rendering
{
	Fence::Fence(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"Fence",
			startPosition,
			startRotation,
			startScale)
	{
	}


	Fence::~Fence()
	{
	}
}