#pragma once

#include "DrawableGameComponent.h"
#include "AnimatedGameObject.h"
#include "GameTime.h"

namespace Rendering
{
	class FarbaMan :
		public AnimatedGameObject
	{
	public:
		FarbaMan(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(0, -9.75, 0),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.05, 0.05, 0.05));
		~FarbaMan();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;

		void setSelection(bool selection);
		bool getIsSelected();
		virtual void CheckTriggers() override;
		virtual void SetAnimations() override;

		void StartPainting();

		bool mAllowPainting;
		bool destroyPaintedPosition;
	private:
		bool isSelected = false;
		bool painting;
		float paintingTime = -1.f;
		bool inPaintArea = false;
	};
}