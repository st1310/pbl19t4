#include "Information.h"

namespace Rendering
{
	Information::Information(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"Information",
			L"Content\\Effects\\TextureMapping.cso",
			startPosition,
			startRotation,
			startScale)
	{
	}


	Information::~Information()
	{
	}
}