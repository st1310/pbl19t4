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
	class MultipleLightsBonesMaterial;
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
		void BuildSphere(float radius);

		virtual void SetAnimations();
		void ChangeAnimation(std::string animationName);

		void RunInit();

	protected:
		std::map<std::string, int> mAnimations;

	private:
		AnimatedGameObject();
		AnimatedGameObject(const AnimatedGameObject& rhs);
		AnimatedGameObject& operator=(const AnimatedGameObject& rhs);

		void UpdateOptions();

		AnimationPlayer* mAnimationPlayer;
		MultipleLightsBonesMaterial* mMaterial;

		//Animation sequence
		AnimationSequence* mAnimationSequence;
		std::string mCurrentAnimation = "Idle";
	};
}
