#include "DayLevel.h"

namespace Rendering
{
	DayLevel::DayLevel(Game& game, Camera& camera)
		:Scene(DAY_LEVEL)
	{
		Size = 0;
		Start(game, camera);
	}


	DayLevel::~DayLevel()
	{
	}

	void DayLevel::Start(Game& game, Camera& camera)
	{
		//GreenSoldier* greenSoldier = new GreenSoldier(game, camera, XMFLOAT3());
		//this->GameObjects.push_back(greenSoldier);

	}
}


