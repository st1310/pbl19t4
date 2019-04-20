#include "TrainLevel.h"

const int TRAIN_LEVEL = 2;

namespace Rendering
{
	TrainLevel::TrainLevel(Game& game, Camera& camera)
		:Scene(TRAIN_LEVEL)
	{
		Start(game, camera);
	}


	TrainLevel::~TrainLevel()
	{
	}

	void TrainLevel::Start(Game& game, Camera& camera)
	{
		Earth* earth1 = new Earth(game, camera);
		this->GameObjects.push_back(earth1);
	}
}


