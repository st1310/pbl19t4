#include "Track.h"



Track::Track(Game& game, Camera& camera)
	: TexturedModelDemo(game, camera,
		"Content\\Models\\Track.obj",
		L"Content\\Textures\\Track.jpg",
		L"Content\\Effects\\TextureMapping.fx")
{
}

Track::~Track()
{
}
