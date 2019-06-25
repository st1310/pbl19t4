#include "Trace2.h"

namespace Rendering
{
	Trace2::Trace2(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"Trace2",
			startPosition,
			startRotation,
			startScale)
	{
	}

	Trace2::~Trace2()
	{
	}
}