#pragma once

#include "Common.h"
#include "CollisionNode.h"

namespace Library
{
	class State
	{
	public:
		State();
		~State();

		void MoveInit(XMFLOAT2 currentPosition, std::vector<XMFLOAT2> positions, float mHeight, float rotation, float translationSpeed, float rotationSpeed, Colliders* collider, CollisionNode* collNode);

		XMFLOAT3 Move();
		XMFLOAT3 Rotate();
		bool IsInActiveState();

	private:
		void ChangeToNextNode();
		void MoveInitNextNode(float targetXMovement, float targetYMovement, float movementSpeed, float rotationSpeed);
		float SetStandarizedRotation(float rotation);
		double GetFixedFloat(float value);

		bool mIsInActiveState = false;
		float height;

		std::vector<XMFLOAT2> mPositions;
		Colliders* mCollider;
		CollisionNode* mCollNode;

		int mCurrentFrameNumber = 0;
		int mFramesCountInState = 0;

		float mTranslationSpeed = 0.0;
		float mRotationSpeed = 0.0;

		float mXDistancePerFrame = 0.0;
		float mYDistancePerFrame = 0.0;

		float mCurrentXPosition = 0.0;
		float mCurrentYPosition = 0.0;

		float mCurrentRotation = 0.0;
		float mTargetRotation = 0.0;
	};
}