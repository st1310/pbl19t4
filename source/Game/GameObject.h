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
	class GameObject : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(GameObject, DrawableGameComponent)

	public:
		GameObject(Game& game, Camera& camera, const std::string modelName, const std::wstring shaderName, XMFLOAT4 position);
		~GameObject();
		
		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		// Transformations etc
		void Scale(float x, float y, float z);
		void Rotate(float x, float y, float z);
		void Rotate(float angle, int axis);
		void Translate(float x, float y, float z);
		void Translate(float value, int axis);

	private:
		GameObject();
		GameObject(const GameObject& rhs);
		GameObject& operator=(const GameObject& rhs);

		void UpdateOptions();

		Effect* mEffect;
		SkinnedModelMaterial* mMaterial;
		const std::string mModelName;
		const std::wstring mShaderName;

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
