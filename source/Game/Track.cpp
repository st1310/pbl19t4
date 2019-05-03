#include "Track.h"

namespace Rendering
{
	Track::Track(Game& game, Camera& camera, 
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation , 
		XMFLOAT3 startScale)
		: GameObject(game, camera, TRACK,
			"Content\\Models\\Track.fbx",
			L"Content\\Effects\\SkinnedModel.cso",
			startPosition,
			startRotation,
			startScale)
	{
	}

	Track::~Track()
	{
	}
}