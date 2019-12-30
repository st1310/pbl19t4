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
			startScale, true)
	{
	}

	Bench::~Bench()
	{
	}

	void Bench::Initialize()
	{
		StaticGameObject::Initialize();
		//TODO: apply rotation
		StaticGameObject::BuildBoundingBox(XMFLOAT3(6.f, 3.9f, 3.f));
	}
}