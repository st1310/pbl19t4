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
		GameObject(Game& game, Camera& camera, const char *modelName, LPCWSTR shaderName, XMFLOAT3 startPosition, XMFLOAT3 startRotation, XMFLOAT3 startScale);
		~GameObject();
		
		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		// Transformations etc
		void Scale(float x, float y, float z);
		void Scale(XMFLOAT3 scale);

		void Rotate(float x, float y, float z);
		void Rotate(float angle, int axis);
		void Rotate(XMFLOAT3 rotation);

		void Translate(float x, float y, float z);
		void Translate(float value, int axis);
		void Translate(XMFLOAT3 translation);

	private:
		GameObject();
		GameObject(const GameObject& rhs);
		GameObject& operator=(const GameObject& rhs);

		void UpdateOptions();

		Effect* mEffect;
		SkinnedModelMaterial* mMaterial;
		const char *mModelName;
		LPCWSTR mShaderName;

		XMFLOAT3 mStartPosition;
		XMFLOAT3 mStartRotation;
		XMFLOAT3 mScale;

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
