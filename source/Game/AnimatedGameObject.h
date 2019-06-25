#pragma once

#include "GameObject.h"
#include "RenderStateHelper.h"
#include "CollisionNode.h"

using namespace Library;

namespace Library
{
	class KeyboardComponent;
	class Effect;
	class SkinnedModelMaterial;
	class Model;
	class AnimationPlayer;
	class AnimationSequence;
}

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

namespace Rendering
{
	class AnimatedGameObject : public GameObject
	{
		RTTI_DECLARATIONS(AnimatedGameObject, GameObject)

	public:
		AnimatedGameObject(Game& game, Camera& camera, const char *className,
			XMFLOAT3 startPosition, XMFLOAT3 startRotation, XMFLOAT3 startScale);
		~AnimatedGameObject();
		
		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		virtual void BuildBoundingBox(XMFLOAT3 radius) override;

		virtual void SetAnimations();
		void ChangeAnimation(std::string animationName);

		void RunInit();
		void PatrolInit();

	protected:
		std::map<std::string, int> mAnimations;

	private:
		AnimatedGameObject();
		AnimatedGameObject(const AnimatedGameObject& rhs);
		AnimatedGameObject& operator=(const AnimatedGameObject& rhs);

		void UpdateOptions();
		XMFLOAT3 GetFollowPositionToCamera();

		AnimationPlayer* mAnimationPlayer;
		SkinnedModelMaterial* mMaterial;

		bool mIsFolowable = false;		

		//Animation sequence
		AnimationSequence* mAnimationSequence;
		std::string mCurrentAnimation = "Idle";
	};
}
