#include "OverheadLines.h"

namespace Rendering
{
	OverheadLines::OverheadLines(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"OverheadLines",
			"Content\\Models\\OverheadLines.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\OverheadLinesDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale, true)
	{
	}


	OverheadLines::~OverheadLines()
	{
	}

	void OverheadLines::Initialize()
	{
		StaticGameObject::Initialize();
		StaticGameObject::BuildBoundingBox(XMFLOAT3(6.3f, 8.f, 9.f));
	}
}