#include "Bench.h"


namespace Rendering
{
	Bench::Bench(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"Bench",
			"Content\\Models\\Bench.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\BenchDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale)
	{
	}

	Bench::~Bench()
	{
	}
}