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
		if (!mStartSequence && mIsInit && mAnimationTimeInLastFrame > currentAnimationTime)
		{
			mStartSequence = true;
		}

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

	void AnimationSequence::EndLoop()
	{
		mIsInLoop = false;
	}
}