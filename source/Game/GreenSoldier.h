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
		bool getArchiveFarbaManPoint();
		virtual void CheckTriggers() override;
		virtual void SetAnimations() override;


		PointLight* GetPointLight();
		SpotLight* GetSpotLight();

	private:
		bool isSelected = false;
		PointLight* mPointLight;
		SpotLight* mSpotLight;
		bool achiveFarbaManPoint = false;
	};
}