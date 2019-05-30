#include "PoliceCar.h"

namespace Rendering
{
	PoliceCar::PoliceCar(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"PoliceCar",
			L"Content\\Effects\\TextureMapping.cso",
			startPosition,
			startRotation,
			startScale, true)
	{
	}

	PoliceCar::~PoliceCar()
	{
	}

	void PoliceCar::Initialize()
	{

	}
}