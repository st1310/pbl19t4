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

		virtual void CheckTriggers() override;
		virtual void SetAnimations() override;

		void addPointToPatrolPath(XMFLOAT2 point);
		void StartFollow();

	private:
		int policeNearby;
		int playerNearby;
		std::vector<XMFLOAT2> patrolPath;
	};
}