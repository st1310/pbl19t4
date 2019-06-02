#include "Track.h"

namespace Rendering
{
	Track::Track(Game& game, Camera& camera, 
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation , 
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera, 
			"Track",
			startPosition,
			startRotation,
			startScale)
	{
	}

	Track::~Track()
	{
	}
}