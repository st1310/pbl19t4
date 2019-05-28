#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class PlatformNumber2And3 :
		public StaticGameObject
	{
	public:
		PlatformNumber2And3(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(0, 19.5, 0),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.85, 0.85, 0.85));
		~PlatformNumber2And3();

		virtual void Initialize() override;
	};
}