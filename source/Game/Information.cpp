#include "Information.h"

namespace Rendering
{
	Information::Information(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"Information",
			"Content\\Models\\Information.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\CityLevelActiveDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale)
	{
	}


	Information::~Information()
	{
	}
}