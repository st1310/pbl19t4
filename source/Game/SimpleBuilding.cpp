#include "SimpleBuilding.h"

namespace Rendering
{
	SimpleBuilding::SimpleBuilding(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"SimpleBuilding",
			startPosition,
			startRotation,
			startScale)
	{
	}


	SimpleBuilding::~SimpleBuilding()
	{
	}
}