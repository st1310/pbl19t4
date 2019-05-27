#include "State.h"
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
			mCurrentFrameNumber++;
			return result;
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

	void State::MoveInit(std::vector<XMFLOAT2> positions, float rotation, float translationSpeed, float rotationSpeed)
	{
		mIsInActiveState = true;

		mPositions = positions;
		mTranslationSpeed = translationSpeed;
		mRotationSpeed = rotationSpeed;
		mCurrentRotation = SetStandarizedRotation(rotation);

		MoveInitNextNode(mPositions.at(0).x, mPositions.at(0).y, mTranslationSpeed, mRotationSpeed);
	}

	void State::ChangeToNextNode()
	{
		mPositions.erase(mPositions.begin());

		mCurrentFrameNumber = 0;

		if (mPositions.size() != 0)
			MoveInitNextNode(mPositions.at(0).x, mPositions.at(0).y, mTranslationSpeed, mRotationSpeed);

		else
			mIsInActiveState = false;
	}

	void State::MoveInitNextNode(float targetXMovement, float targetYMovement, float movementSpeed, float rotationSpeed)
	{
		float distanceSquare = pow(targetXMovement, 2) + pow(targetYMovement, 2);
		float distance = sqrt(distanceSquare);
		mCurrentFrameNumber = 0;

		if (rotationSpeed != 0)
		{
			if (targetXMovement > 0 && targetYMovement == 0)
				mTargetRotation = 90;

			else if (targetXMovement < 0 && targetYMovement == 0)
				mTargetRotation = -90;

			else if (targetYMovement > 0 && targetXMovement == 0)
				mTargetRotation = 0;

			else if (targetYMovement < 0 && targetXMovement == 0)
				mTargetRotation = 180;

			else
			{
				if (targetXMovement == targetYMovement)
					mTargetRotation = 45;

				else
				{
					// if |x| < |y| tangens has wrong value
					double x = (abs(targetXMovement) > abs(targetYMovement)) ? abs(targetXMovement) : abs(targetYMovement);
					double y = (abs(targetXMovement) > abs(targetYMovement)) ? abs(targetYMovement) : abs(targetXMovement);

					double targetRotationTangens = x / y;
					double targetRotationInRadians = tan(targetRotationTangens);

					mTargetRotation = 90 - XMConvertToDegrees(targetRotationInRadians);
				}

				// Check in which quarter is degree
				if (targetXMovement > 0 && targetYMovement < 0)
					mTargetRotation += 90;

				if (targetXMovement < 0 && targetYMovement > 0)
					mTargetRotation = -mTargetRotation;

				if (targetXMovement < 0 && targetYMovement < 0)
					mTargetRotation = -mTargetRotation - 90;
			}
		}

		if (movementSpeed != 0)
		{
			mFramesCountInState = ceil(distance / movementSpeed);
			mXDistancePerFrame = targetXMovement / mFramesCountInState;
			mYDistancePerFrame = targetYMovement / mFramesCountInState;

			mCurrentXPosition = 0.0;
			mCurrentYPosition = 0.0;
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
}