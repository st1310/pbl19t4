#include "CityLevel.h"

namespace Rendering
{
	CityLevel::CityLevel(Game& game, Camera& camera)
		:Scene(CITY_LEVEL)
	{
		Size = 2;
		Start(game, camera);
	}


	CityLevel::~CityLevel()
	{
	}

	void CityLevel::Start(Game& game, Camera& camera)
	{
		GreenSoldier* greenSoldier1 = new GreenSoldier(game, camera, XMFLOAT3());
		XMFLOAT3 greenSoldier2Translate = XMFLOAT3(10, 0, 0);
		GreenSoldier* greenSoldier2 = new GreenSoldier(game, camera, greenSoldier2Translate);
		//PassengerTrain* passegnerTrain1 = new PassengerTrain(game, camera, XMFLOAT3());
		this->GameObjects.push_back(greenSoldier1);
		this->GameObjects.push_back(greenSoldier2);
	}
}


