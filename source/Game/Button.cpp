#include "Button.h"

namespace Rendering
{
	Button::Button(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera, 
			"",
			"Content\\Models\\Sphere.obj",
			L"Content\\Effects\\SkinnedModel.cso",
			"",
			startPosition,
			startRotation,
			startScale)
	{
	}


	Button::~Button()
	{
	}
}