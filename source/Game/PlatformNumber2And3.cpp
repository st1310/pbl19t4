#include "PlatformNumber2And3.h"

namespace Rendering
{
	PlatformNumber2And3::PlatformNumber2And3(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"PlatformNumber2And3",
			"Content\\Models\\PlatformNumber2And3.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\PlatformNumber2And3DiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale, true)
	{
	}


	PlatformNumber2And3::~PlatformNumber2And3()
	{
	}

	void PlatformNumber2And3::Initialize()
	{
		StaticGameObject::Initialize();
		StaticGameObject::BuildBoundingBox(XMFLOAT3(6.3f, 8.f, 9.f));
	}
}