#include "DayLevel.h"

namespace Rendering
{
	DayLevel::DayLevel(Game& game, Camera& camera)
		:Scene(DAY_LEVEL, "day.txt")
	{
		Start(game, camera);
	}


	DayLevel::~DayLevel()
	{
	}

	void DayLevel::Start(Game& game, Camera& camera)
	{
		GreenSoldier* greenSoldier1 = new GreenSoldier(game, camera, XMFLOAT3());
		XMFLOAT3 greenSoldier2Translate = XMFLOAT3(10, 0, 0);
		GreenSoldier* greenSoldier2 = new GreenSoldier(game, camera, greenSoldier2Translate);
		//PassengerTrain* passegnerTrain1 = new PassengerTrain(game, camera, XMFLOAT3());
		this->GameObjects.push_back(greenSoldier1);
		this->GameObjects.push_back(greenSoldier2);

	}
}


