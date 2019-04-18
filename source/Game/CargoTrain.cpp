#include "CargoTrain.h"



CargoTrain::CargoTrain(Game& game, Camera& camera)
	: TexturedModelDemo(game, camera,
		"Content\\Models\\CargoTrain.obj",
		L"Content\\Textures\\CargoTrain.jpg",
		L"Content\\Effects\\TextureMapping.fx")
{
}


CargoTrain::~CargoTrain()
{
}
