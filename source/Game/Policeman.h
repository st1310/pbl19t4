#pragma once

#include "DrawableGameComponent.h"
#include "AnimatedGameObject.h"
#include "StaticGameObject.h"

namespace Rendering
{
	class Policeman :
		public AnimatedGameObject
	{
	public:
		Policeman(Game& game, Camera& camera,
			XMFLOAT3 startPosition = XMFLOAT3(0, -10, 0),
			XMFLOAT3 startRotation = XMFLOAT3(-90, 0, 0),
			XMFLOAT3 startScale = XMFLOAT3(0.05, 0.05, 0.05));
		~Policeman();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;

		virtual void CheckTriggers() override;
		virtual void SetAnimations() override;

		void Patrol(std::vector<XMFLOAT2> patrolPoints);

		void addPointToPatrolPath(XMFLOAT2 point);
		void StartFollow();

		PointLight* GetPointLight();
		SpotLight* GetSpotLight();

	private:
		int policeNearby;
		int playerNearby;
		std::vector<XMFLOAT2> patrolPath;
		PointLight* mPointLight;
		SpotLight* mSpotLight;
		bool mRunAndCatchUnit = false;
	};
}