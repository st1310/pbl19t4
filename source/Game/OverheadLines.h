#pragma once

#include "DrawableGameComponent.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class OverheadLines :
		public StaticGameObject
	{
	public:
		OverheadLines(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(0, 36, 0),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(1.4, 1.4, 1.4));
		~OverheadLines();

		virtual void Initialize() override;
	};
}
