#include "Earth.h"



Earth::Earth(Game& game, Camera& camera)
	: TexturedModelDemo(game, camera, 
		"Content\\Models\\Sphere.obj", 
		L"Content\\Textures\\EarthComposite.jpg", 
		L"Content\\Effects\\TextureMapping.fx")
{
}


Earth::~Earth()
{
}
