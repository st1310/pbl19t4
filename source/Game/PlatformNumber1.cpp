#include "PlatformNumber1.h"

namespace Rendering
{
	PlatformNumber1::PlatformNumber1(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"PlatformNumber1",
			"Content\\Models\\PlatformNumber1.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\PlatformNumber1DiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale, true)
	{
	}


	PlatformNumber1::~PlatformNumber1()
	{
	}

	void PlatformNumber1::Initialize()
	{
		StaticGameObject::Initialize();
		StaticGameObject::BuildBoundingBox(XMFLOAT3(6.3f, 8.f, 9.f));
	}
}