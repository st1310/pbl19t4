#include "State.h"
#include "NodeList.h"
#include <cmath>

namespace Library
{
	State::State()
	{
	}


	State::~State()
	{
	}

	bool State::IsInActiveState()
	{
		return mIsInActiveState;
	}

	XMFLOAT3 State::Move()
	{
		if (mCurrentFrameNumber < mFramesCountInState)
		{
			XMFLOAT3 result = XMFLOAT3(mXDistancePerFrame, 0, mYDistancePerFrame);

			XMMATRIX matrixRotation = XMMATRIX({ 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f });

			if (NodeList::TryToMoveInNode(mCollider, XMVECTOR({ mCurrentXPosition, -7.0f, mCurrentYPosition, 1.0f }), matrixRotation, XMVECTOR({ mCurrentXPosition + mXDistancePerFrame, -7.0f, mCurrentYPosition + mYDistancePerFrame, 1.0f }), mCollNode))
			{
				mCurrentFrameNumber++;
				mCurrentXPosition += mXDistancePerFrame;
				mCurrentYPosition += mYDistancePerFrame;

				return result;
			}		
		}

		ChangeToNextNode();
		XMFLOAT3 result = XMFLOAT3(0, 0, 0);
		return result;
	}

	XMFLOAT3 State::Rotate()
	{
		XMFLOAT3 result = XMFLOAT3(0, 0, 0);

		if (mCurrentRotation < mTargetRotation)
		{
			if (mCurrentRotation + mRotationSpeed > mTargetRotation)
			{
				float rotation = mTargetRotation - mCurrentRotation;
				result = XMFLOAT3(0, rotation, 0);
			}

			else
				result = XMFLOAT3(0, mRotationSpeed, 0);
		}

		else if (mCurrentRotation > mTargetRotation)
		{
			if (mCurrentRotation - mRotationSpeed < mTargetRotation)
			{
				float rotation = mTargetRotation - mCurrentRotation;
				result = XMFLOAT3(0, rotation, 0);
			}

			else
				result = XMFLOAT3(0, -mRotationSpeed, 0);
		}

		mCurrentRotation += result.y;
		return result;
	}

	void State::MoveInit(XMFLOAT2 currentPosition,  std::vector<XMFLOAT2> positions, float rotation, float translationSpeed, float rotationSpeed, 
		Colliders* collider, CollisionNode* collNode, bool isLoopable)
	{
		mCollider = collider;
		mCollNode = collNode;
		mIsInActiveState = true;
		mIsLoopable = isLoopable;

		mCurrentXPosition = currentPosition.x;
		mCurrentYPosition = currentPosition.y;

		mPositions = positions;
		mTranslationSpeed = translationSpeed;
		mRotationSpeed = rotationSpeed;
		mCurrentRotation = SetStandarizedRotation(rotation);

		MoveInitNextNode(mPositions.at(0).x, mPositions.at(0).y, mTranslationSpeed, mRotationSpeed);
	}

	void State::ChangeToNextNode()
	{
		if (mIsLoopable)
		{
			mPositions.push_back(mPositions.at(0));
			mPositions.erase(mPositions.begin());
		}

		else
			mPositions.erase(mPositions.begin());

		mCurrentFrameNumber = 0;

		if (mPositions.size() != 0)
			MoveInitNextNode(mPositions.at(0).x, mPositions.at(0).y, mTranslationSpeed, mRotationSpeed);

		else
			mIsInActiveState = false;
	}

	void State::MoveInitNextNode(float targetXMovement, float targetYMovement, float movementSpeed, float rotationSpeed)
	{
		float xDistance = GetFixedFloat(targetXMovement - mCurrentXPosition);
		float yDistance = GetFixedFloat(targetYMovement - mCurrentYPosition);

		float distanceSquare = pow(xDistance, 2) + pow(yDistance, 2);
		float distance = sqrt(distanceSquare);
		mCurrentFrameNumber = 0;

		if (rotationSpeed != 0)
		{
			if (xDistance > 0 && yDistance == 0)
				mTargetRotation = 90;

			else if (xDistance < 0 && yDistance == 0)
				mTargetRotation = -90;

			else if (yDistance > 0 && xDistance == 0)
				mTargetRotation = 0;

			else if (yDistance < 0 && xDistance == 0)
				mTargetRotation = 180;

			else
			{
				if (xDistance == yDistance)
					mTargetRotation = 45;

				else
				{
					// if |x| < |y| tangens has wrong value
					double x = (abs(xDistance) > abs(yDistance)) ? abs(xDistance) : abs(yDistance);
					double y = (abs(xDistance) > abs(yDistance)) ? abs(yDistance) : abs(xDistance);

					double targetRotationTangens = x / y;
					double targetRotationInRadians = tan(targetRotationTangens);
				}

				// Check in which quarter is degree
				if (xDistance > 0 && yDistance < 0)
					mTargetRotation += 90;

				if (xDistance < 0 && yDistance > 0)
					mTargetRotation = -mTargetRotation;

				if (xDistance < 0 && yDistance < 0)
					mTargetRotation = -mTargetRotation - 90;
			}
		}

		if (movementSpeed != 0)
		{
			mFramesCountInState = ceil(distance / movementSpeed);
			mXDistancePerFrame = xDistance / mFramesCountInState;
			mYDistancePerFrame = yDistance / mFramesCountInState;
		}
	}

	float State::SetStandarizedRotation(float rotation)
	{
		if (rotation > 180)
			rotation = rotation - 180;

		if (rotation < -180)
			rotation = rotation + 180;

		return rotation;
	}

	double State::GetFixedFloat(float value)
	{
		float rounded = ((int)(value * 100 + .5) / 100.0);

		return rounded;
	}
}