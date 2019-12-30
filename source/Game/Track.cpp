#include "Track.h"

namespace Rendering
{
	Track::Track(Game& game, Camera& camera, 
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation , 
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera, 
			"Track",
			"Content\\Models\\Track.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\TrackDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale)
	{
	}

	Track::~Track()
	{
	}
}