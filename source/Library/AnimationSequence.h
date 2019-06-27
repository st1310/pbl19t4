#pragma once

#include "GameComponent.h"

namespace Library
{
	class AnimationSequence : GameComponent
	{
		RTTI_DECLARATIONS(AnimationSequence, GameComponent)

	public:
		AnimationSequence(std::string idleAnimation);
		~AnimationSequence();

		void InitLoopAnimationSequence(std::string initAnimation, std::string loopAnimation, std::string endAnimation);
		void InitLoopAnimationSequence(std::string initAnimation, std::string loopAnimation);
		void InitConstAnimationSequence(std::string initAnimation, std::string loopAnimation, std::string endAnimation, int repeatSum);

		std::string GetCurrentAnimation(float currentAnimationTime);
		void EndLoop();

	private:
		void SetLoopableAnimation(float currentAnimationTime);
		void SetConstAnimation(float currentAnimationTime);

		std::string mIdleAnimation;		
		std::string mCurrentAnimation;

		std::string mInitAnimation;
		std::string mLoopAnimation;
		std::string mEndAnimation;

		bool mIsIdle = true;
		bool mIsInit = false;;
		bool mIsInLoop = false;
		bool mIsEnd = false;

		bool mStartSequence = false;
		bool mCurrentRepeatSum = 0;
		bool mMaxRepeatSum = 0;

		float mAnimationTimeInLastFrame = 0;

	};
}