#include "Earth.h"

namespace Rendering
{
	Earth::Earth(Game& game, Camera& camera, XMFLOAT4 position)
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