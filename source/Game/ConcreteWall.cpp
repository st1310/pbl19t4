#include "ConcreteWall.h"

namespace Rendering
{
	ConcreteWall::ConcreteWall(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"ConcreteWall",
			"Content\\Models\\ConcreteWall.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\ConcreteWallDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale, true)
	{
	}


	ConcreteWall::~ConcreteWall()
	{
	}

	void ConcreteWall::Initialize()
	{
		StaticGameObject::Initialize();
		StaticGameObject::BuildBoundingBox(XMFLOAT3(6.3f, 8.f, 9.f));
	}
}