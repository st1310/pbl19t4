#include "PassengerTrain.h"



PassengerTrain::PassengerTrain(Game& game, Camera& camera)
	: TexturedModelDemo(game, camera,
		"Content\\Models\\PassengerTrain.obj",
		L"Content\\Textures\\PassengerTrain.jpg",
		L"Content\\Effects\\TextureMapping.fx")
{
}


PassengerTrain::~PassengerTrain()
{
}
