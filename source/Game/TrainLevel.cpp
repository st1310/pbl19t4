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
		GreenSoldier* greenSoldier1 = new GreenSoldier(game, camera, XMFLOAT3());
		XMFLOAT3 greenSoldier2Translate = XMFLOAT3(10, 0, 0);
		GreenSoldier* greenSoldier2 = new GreenSoldier(game, camera, greenSoldier2Translate);
		//CargoTrain* passegnerTrain1 = new CargoTrain(game, camera, XMFLOAT4());
		this->GameObjects.push_back(greenSoldier1);
		this->GameObjects.push_back(greenSoldier2);
		//this->GameObjects.push_back(passegnerTrain1);		
	}
}


