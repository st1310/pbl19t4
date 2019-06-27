#include "AnimationSequence.h"


namespace Library
{
	RTTI_DEFINITIONS(AnimationSequence)

	AnimationSequence::AnimationSequence(std::string idleAnimation) :
		mIdleAnimation(idleAnimation)
	{
	}


	AnimationSequence::~AnimationSequence()
	{
	}

	std::string AnimationSequence::GetCurrentAnimation(float currentAnimationTime)
	{
		if (mMaxRepeatSum == 0)
			SetLoopableAnimation(currentAnimationTime);

		else
			SetConstAnimation(currentAnimationTime);

		return mCurrentAnimation;
	}

	void AnimationSequence::InitLoopAnimationSequence(std::string initAnimation, std::string loopAnimation, std::string endAnimation)
	{
		mInitAnimation = initAnimation;
		mLoopAnimation = loopAnimation;
		mEndAnimation = endAnimation;

		mCurrentAnimation = mInitAnimation;
		mAnimationTimeInLastFrame = 0;

		mIsIdle = false;
		mIsInit = true;
		mIsInLoop = false;
		mIsEnd = false;
		mStartSequence = false;
	}

	void AnimationSequence::InitLoopAnimationSequence(std::string initAnimation, std::string loopAnimation)
	{
		mInitAnimation = initAnimation;
		mLoopAnimation = loopAnimation;

		mCurrentAnimation = mInitAnimation;
		mAnimationTimeInLastFrame = 0;

		mIsIdle = false;
		mIsInit = true;
		mIsInLoop = false;
		mIsEnd = false;
		mStartSequence = false;
	}

	void AnimationSequence::InitConstAnimationSequence(std::string initAnimation, std::string loopAnimation, std::string endAnimation, int repeatSum)
	{
		mInitAnimation = initAnimation;
		mLoopAnimation = loopAnimation;
		mEndAnimation = endAnimation;

		mCurrentAnimation = mInitAnimation;
		mAnimationTimeInLastFrame = 0;

		mIsIdle = false;
		mIsInit = true;
		mIsInLoop = false;
		mIsEnd = false;
		mStartSequence = false;

		mCurrentRepeatSum = 0;
		mMaxRepeatSum = repeatSum;
	}

	void AnimationSequence::EndLoop()
	{
		mIsInLoop = false;
	}

	void AnimationSequence::SetLoopableAnimation(float currentAnimationTime)
	{
		if (!mStartSequence && mIsInit && mAnimationTimeInLastFrame > currentAnimationTime)
			mStartSequence = true;

		else if (mIsInit && mAnimationTimeInLastFrame > currentAnimationTime)
		{
			mIsInit = false;
			mIsInLoop = true;
			mCurrentAnimation = mLoopAnimation;
		}

		else if (!mIsInLoop && !mIsEnd && !mIsIdle && mAnimationTimeInLastFrame > currentAnimationTime)
		{
			mIsEnd = true;
			mCurrentAnimation = mEndAnimation;
		}

		else if (mIsEnd && !mIsIdle && mAnimationTimeInLastFrame > currentAnimationTime)
		{
			mIsEnd = false;
			mIsIdle = true;
			mCurrentAnimation = mIdleAnimation;
		}

		mAnimationTimeInLastFrame = currentAnimationTime;
	}

	void AnimationSequence::SetConstAnimation(float currentAnimationTime)
	{
		if (!mIsInit && mAnimationTimeInLastFrame > currentAnimationTime)
			mCurrentRepeatSum++;

		if (mCurrentRepeatSum == mMaxRepeatSum)
			EndLoop();

		SetLoopableAnimation(currentAnimationTime);
	}
}