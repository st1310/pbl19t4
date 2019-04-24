#include "Track.h"

namespace Rendering
{
	Track::Track(Game& game, Camera& camera, XMFLOAT3 position)
		: GameObject(game, camera,
			"Content\\Models\\Track.fbx",
			L"Content\\Effects\\SkinnedModel.cso",
			position)
	{
	}

	Track::~Track()
	{
	}
}