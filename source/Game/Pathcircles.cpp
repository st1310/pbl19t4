#include "Pathcircles.h"


namespace Rendering
{
	Pathcircles::Pathcircles(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"Bench",
			"Content\\Models\\Pathcircles.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\BenchDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale, true)
	{
	}

	Pathcircles::~Pathcircles()
	{
	}

	void Pathcircles::Initialize()
	{
		StaticGameObject::Initialize();
		//TODO: apply rotation
		StaticGameObject::BuildBoundingBox(XMFLOAT3(6.f, 3.9f, 3.f));
	}
}