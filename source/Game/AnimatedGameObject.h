#pragma once

#include "DrawableGameComponent.h"
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
	class AnimatedGameObject : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(AnimatedGameObject, DrawableGameComponent)

	public:
		AnimatedGameObject(Game& game, Camera& camera, const char *className,
			const char *modelName, LPCWSTR shaderName, std::string diffuseMap,
			XMFLOAT3 startPosition, XMFLOAT3 startRotation, XMFLOAT3 startScale);
		~AnimatedGameObject();
		
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
		
		XMFLOAT3 getPosition();
		Colliders* getCollider();
		void SetNode(CollisionNode* colNode);
		CollisionNode* getNode();

		virtual void CheckTriggers();

		bool mIsSelected = true;
		bool mIsEdited = false;


	private:
		AnimatedGameObject();
		AnimatedGameObject(const AnimatedGameObject& rhs);
		AnimatedGameObject& operator=(const AnimatedGameObject& rhs);

		void UpdateOptions();

		Effect* mEffect;
		SkinnedModelMaterial* mMaterial;
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

		Colliders* mDynCollider;
		CollisionNode* inNode;

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
	};
}
