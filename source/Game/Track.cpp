#include "Track.h"



Track::Track(Game& game, Camera& camera, double x, double y)
	: TexturedModelDemo(game, camera,
		"Content\\Models\\Track.obj",
		L"Content\\Textures\\Track.jpg",
		L"Content\\Effects\\TextureMapping.fx",
		x,
		y)
{
}

Track::~Track()
{
}
