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
			const char *modelName, LPCWSTR shaderName, std::string diffuseMap,
			XMFLOAT3 startPosition, XMFLOAT3 startRotation, XMFLOAT3 startScale);
		~AnimatedGameObject();
		
		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		virtual void BuildBoundingBox(XMFLOAT3 radius) override;

	private:
		AnimatedGameObject();
		AnimatedGameObject(const AnimatedGameObject& rhs);
		AnimatedGameObject& operator=(const AnimatedGameObject& rhs);

		void UpdateOptions();

		AnimationPlayer* mAnimationPlayer;
		SkinnedModelMaterial* mMaterial;
	};
}
