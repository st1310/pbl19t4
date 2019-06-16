#include "TrainStationMap.h"

namespace Rendering
{
	TrainStationMap::TrainStationMap(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"TrainStationMap",
			startPosition,
			startRotation,
			startScale)
	{
	}

	TrainStationMap::~TrainStationMap()
	{
	}
}