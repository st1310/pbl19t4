#include "Button.h"

namespace Rendering
{
	Button::Button(Game& game, Camera& camera, XMFLOAT3 position)
		: GameObject(game, camera,
			"Content\\Models\\Sphere.obj",
			L"Content\\Effects\\SkinnedModel.cso",
			position)
	{
	}


	Button::~Button()
	{
	}
}