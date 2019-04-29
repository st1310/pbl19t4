#include "Earth.h"

namespace Rendering
{
	Earth::Earth(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: GameObject(game, camera,
			"Content\\Models\\Button_off.obj",
			L"Content\\Effects\\SkinnedModel.cso",
			startPosition,
			startRotation,
			startScale)
	{
	}


	Earth::~Earth()
	{
	}
}