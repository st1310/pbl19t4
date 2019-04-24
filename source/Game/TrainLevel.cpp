#include "TrainLevel.h"


namespace Rendering
{
	TrainLevel::TrainLevel(Game& game, Camera& camera)
		:Scene(TRAIN_LEVEL)
	{
		Size = 2;
		Start(game, camera);
	}


	TrainLevel::~TrainLevel()
	{
	}

	void TrainLevel::Start(Game& game, Camera& camera)
	{
		Earth* earth1 = new Earth(game, camera, XMFLOAT4());
		PassengerTrain* passegnerTrain1 = new PassengerTrain(game, camera, XMFLOAT4());
		this->GameObjects.push_back(earth1);
		this->GameObjects.push_back(passegnerTrain1);		
	}
}


