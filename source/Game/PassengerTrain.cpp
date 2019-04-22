#include "PassengerTrain.h"

namespace Rendering
{
	PassengerTrain::PassengerTrain(Game& game, Camera& camera, double x, double y)
		: TexturedModelDemo(game, camera,
			"Content\\Models\\PassengerTrain.obj",
			L"Content\\Textures\\PassengerTrain.jpg",
			L"Content\\Effects\\TextureMapping.fx",
			x,
			y)
	{
	}


	PassengerTrain::~PassengerTrain()
	{
	}
}