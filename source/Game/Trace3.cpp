#include "Trace3.h"

namespace Rendering
{
	Trace3::Trace3(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"Trace3",
			startPosition,
			startRotation,
			startScale)
	{
	}

	Trace3::~Trace3()
	{
	}
}