#pragma once

#include "DrawableGameComponent.h"
#include "RenderStateHelper.h"
#include "State.h"

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

		XMFLOAT3 getPosition();

		Colliders* getCollider();
		void SetNode(CollisionNode* colNode);
		virtual CollisionNode* getNode() override;

		virtual void BuildBoundingBox(XMFLOAT3 radius);
		void BuildOrientedBoundingBox(XMFLOAT3 radius, XMFLOAT4 orientation);

		void setIsSelected(bool value);
		bool getIsSelected();

		bool mIsSelected = false;
		bool mIsEdited = false;

		void StartMoving(std::vector<XMFLOAT2> positions);
		void SetPathFindingMoveFlag(bool value);
		bool GetPathFindingMoveFlag();

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

		//little work around to fix rotation bug
		XMFLOAT3 mOriginalPosition = XMFLOAT3(0, 0, 0);

		//states
		State* mState;
		float mRotationSpeed;
		float mTranslationSpeed;

		bool mIsBusy = false;

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

		Colliders* mCollider;
		CollisionNode* inNode;

		bool objectToMove;

		// Creation Kit		
		std::string mEditMode = POSITION;
		std::string mEditAxis = X_AXIS;
		int mEditFactorIndex = 5;
		int mAxisNumber = X_AXIS_NUMBER;
		boolean mPrecisionMode = false;
		boolean mXYPositionMode = true;
		const char *mClassName;

		void EditModel();
		void ChangeEditMode();
		void ChangeEditAxis();
		void ChangeEditFactor();
		void SetPosition();
		void SetRotation();
		void SetScale();
		std::wostringstream GetCreationKitInfo();

		const float mEditFactorArray[11] = {
			0.01f,
			0.05f,
			0.1f,
			0.25f,
			0.5f,
			1.0f,
			2.5f,
			5.0f,
			25.0f,
			45.0f,
			90.0f
		};

		/*
		CREATION KIT CONTROLS
		1- Position
		2- Rotation
		3- Scale
		4- Precision mode
		5- XY mode (for Position)
		7- Min edit factor
		9- Max edit factor
		'-'- smaller edit factor
		'+'- bigger edit factor
		UPARROW, DOWNARROW- remove/add value
		LEFTARROW, RIGHTARROW - change axis

		FOR XYMODE all arrows editing axis (L,R -> x, U,D -> z)

		'x'- End editing
		*/
	};
}
