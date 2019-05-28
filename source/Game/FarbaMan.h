#pragma once

#include "DrawableGameComponent.h"
#include "AnimatedGameObject.h"

namespace Rendering
{
	class FarbaMan :
		public AnimatedGameObject
	{
	public:
		FarbaMan(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(0, -2.5, 0),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.02, 0.02, 0.02));
		~FarbaMan();

		virtual void Initialize() override;

		void setSelection(bool selection);
		bool getIsSelected();
		virtual void CheckTriggers() override;

	private:
		bool isSelected = false;
	};
}