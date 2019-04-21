#include "CargoTrain.h"



CargoTrain::CargoTrain(Game& game, Camera& camera, double x, double y)
	: TexturedModelDemo(game, camera,
		"Content\\Models\\CargoTrain.obj",
		L"Content\\Textures\\CargoTrain.jpg",
		L"Content\\Effects\\TextureMapping.fx",
		x, 
		y)
{
}


CargoTrain::~CargoTrain()
{
}
