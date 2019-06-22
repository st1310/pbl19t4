#pragma once

#include "DrawableGameComponent.h"
#include "AnimatedGameObject.h"

namespace Rendering
{
	class GreenSoldier :
		public AnimatedGameObject
	{
	public:
		GreenSoldier(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(0, -9.7, 0),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.05, 0.05, 0.05));
		~GreenSoldier();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;

		void setSelection(bool selection);
		bool getIsSelected();
		virtual void CheckTriggers() override;
		virtual void SetAnimations() override;

<<<<<<< HEAD
=======
		PointLight* GetPointLight();
>>>>>>> parent of 5425e12... Trigger, Enter TrainStation to unlock FarbaMan
		SpotLight* GetSpotLight();

	private:
		SpotLight* mSpotLight;
	};
}