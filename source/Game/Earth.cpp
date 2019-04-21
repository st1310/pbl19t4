#include "Earth.h"



Earth::Earth(Game& game, Camera& camera, double x, double y)
	: TexturedModelDemo(game, camera, 
		"Content\\Models\\Sphere.obj", 
		L"Content\\Textures\\EarthComposite.jpg", 
		L"Content\\Effects\\TextureMapping.fx",
		x,
		y)
{
}


Earth::~Earth()
{
}
