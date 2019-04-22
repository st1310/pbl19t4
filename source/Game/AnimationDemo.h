#pragma once

#include "DrawableGameComponent.h"
#include "RenderStateHelper.h"

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
	class AnimationDemo : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(AnimationDemo, DrawableGameComponent)

	public:
		AnimationDemo(Game& game, Camera& camera);
		~AnimationDemo();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		AnimationDemo();
		AnimationDemo(const AnimationDemo& rhs);
		AnimationDemo& operator=(const AnimationDemo& rhs);

		void UpdateOptions();

		Effect* mEffect;
		SkinnedModelMaterial* mMaterial;

		KeyboardComponent* mKeyboard;
		XMFLOAT4X4 mWorldMatrix;

		std::vector<ID3D11Buffer*> mVertexBuffers;
		std::vector<ID3D11Buffer*> mIndexBuffers;
		std::vector<UINT> mIndexCounts;
		std::vector<ID3D11ShaderResourceView*> mColorTextures;

		Model* mSkinnedModel;
		AnimationPlayer* mAnimationPlayer;

		RenderStateHelper mRenderStateHelper;
		SpriteBatch* mSpriteBatch;
		SpriteFont* mSpriteFont;
		XMFLOAT2 mTextPosition;
		bool mManualAdvanceMode;
	};
}
