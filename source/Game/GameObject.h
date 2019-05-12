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
		GameObject(Game& game, Camera& camera, const char *className,
			const char *modelName, LPCWSTR shaderName, std::string diffuseMap,
			XMFLOAT3 startPosition, XMFLOAT3 startRotation, XMFLOAT3 startScale);
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
		bool mIsSelected = false;
		bool mIsEdited = false;

	protected:
		GameObject();
		GameObject(const GameObject& rhs);
		GameObject& operator=(const GameObject& rhs);

		Effect* mEffect;
		const char *mModelName;
		LPCWSTR mShaderName;
		std::string mDiffuseMap;

		XMFLOAT3 mPosition;
		XMFLOAT3 mRotation;
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

		// Creation Kit		
		std::string mEditMode = POSITION;
		std::string mEditAxis = X_AXIS;
		float mEditFactor = 0.5;
		int mAxisNumber = X_AXIS_NUMBER;
		boolean mPrecisionMode = false;
		const char *mClassName;

		void EditModel();
		void ChangeEditMode();
		void ChangeEditAxis();
		void ChangeEditFactor();
		void SetPosition();
		void SetRotation();
		void SetScale();
		std::wostringstream GetCreationKitInfo();
	};
}
