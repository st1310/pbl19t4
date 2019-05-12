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
		GameObject(Game& game, Camera& camera, const char *className, XMFLOAT3 startPosition, XMFLOAT3 startRotation, XMFLOAT3 startScale);
		~GameObject();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		// Transformations etc
		void Scale(float x, float y, float z);
		void Scale(XMFLOAT3 scale);

		void Rotate(float x, float y, float z);
		void Rotate(XMFLOAT3 rotation);
		void FirstRotation();

		void Translate(float x, float y, float z);
		void Translate(XMFLOAT3 translation);

		virtual std::vector<std::string> Serialize() override;

	protected:
		GameObject();
		GameObject(const GameObject& rhs);
		GameObject& operator=(const GameObject& rhs);

		Effect* mEffect;
		SkinnedModelMaterial* mMaterial;
		const char *mModelName;
		LPCWSTR mShaderName;

		XMFLOAT3 mPosition;
		XMFLOAT3 mRotation;
		XMFLOAT3 mScale;

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

		bool mIsSelected = true;

		// Creation Kit
		bool mIsEdited = true;
		std::string mEditMode = POSITION;
		std::string mEditAxis = X_AXIS;
		float mEditFactor = 0.5;
		int mAxisNumber = X_AXIS_NUMBER;
		boolean mPrecisionMode = false;
		const char *mClassName;

		void EditModel(KeyboardComponent* mKeyboard);
		void ChangeEditMode(KeyboardComponent* mKeyboard);
		void ChangeEditAxis(KeyboardComponent* mKeyboard);
		void ChangeEditFactor(KeyboardComponent* mKeyboard);
		void SetPosition(KeyboardComponent* mKeyboard);
		void SetRotation(KeyboardComponent* mKeyboard);
		void SetScale(KeyboardComponent* mKeyboard);
	};
}
