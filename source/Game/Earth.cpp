#include "Earth.h"

namespace Rendering
{
	Earth::Earth(Game& game, Camera& camera, XMFLOAT3 position)
		: GameObject(game, camera,
			"Content\\Models\\Sphere.obj",
			L"Content\\Effects\\SkinnedModel.cso",
			position)
	{
	}


	Earth::~Earth()
	{
	}
}