#pragma once

#include "DrawableGameComponent.h"
#include "RenderStateHelper.h"
#include "State.h"

using namespace Library;

namespace Library
{
	class KeyboardComponent;
	class DirectionalLight;
	class PointLight;
	class SpotLight;
	class ProxyModel;
	class Effect;
	class SkinnedModelMaterial;
	class MultipleLightsMaterial;
	class Model;
	class AnimationPlayer;
	class FullScreenRenderTarget;
	class FullScreenQuad;
	class ColorFilterMaterial;
}

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

namespace Rendering
{
	enum ColorFilter
	{
		ColorFilterGrayScale = 0,
		ColorFilterInverse,
		ColorFilterSepia,
		ColorFilterGeneric,
		ColorFilterEnd
	};

	const std::string ColorFilterTechniqueNames[] = { "grayscale_filter", "inverse_filter", "sepia_filter", "generic_filter" };
	const std::string ColorFilterDisplayNames[] = { "Grayscale", "Inverse", "Sepia", "Generic" };

	class GameObject : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(GameObject, DrawableGameComponent)

		typedef struct _POINT_LIGHT
		{
			XMFLOAT3 Position;
			FLOAT LightRadius;
			XMFLOAT4 Color;
			bool Enabled;

			_POINT_LIGHT() {}

			_POINT_LIGHT(const XMFLOAT3& position, FLOAT radius, const XMFLOAT4& color, bool enabled) :
				Position(position), LightRadius(radius), Color(color), Enabled(enabled) {}
		} POINT_LIGHT;

	public:
		GameObject(Game& game, Camera& camera, const char *className,
			XMFLOAT3 startPosition, XMFLOAT3 startRotation, XMFLOAT3 startScale);
		~GameObject();

		bool IsMustBeDestroyed();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		std::vector<DirectionalLight*> mDirectLights;
		std::vector<PointLight*> mPointLights;
		std::vector<SpotLight*> mSpotLights;
		std::vector<ProxyModel*> mProxyModels;

		XMCOLOR mAmbientColor;
		XMCOLOR mSpecularColor;
		float mSpecularPower;

		ID3D11BlendState* mBlendState;

		FullScreenRenderTarget* mRenderTarget;
		FullScreenQuad* mFullScreenQuad;
		Effect* mColorFilterEffect;
		ColorFilterMaterial* mColorFilterMaterial;
		ColorFilter mActiveColorFilter;
		XMFLOAT4X4 mGenericColorFilter;

		// Transformations etc
		void Scale(float x, float y, float z);
		void Scale(XMFLOAT3 scale);

		void Rotate(float x, float y, float z);
		void Rotate(XMFLOAT3 rotation);
		void FirstRotation();

		void FirstTranslation(XMFLOAT3 translation);
		void Translate(float x, float y, float z);
		void Translate(XMFLOAT3 translation);

		virtual std::vector<std::string> Serialize() override;

		XMFLOAT3 getPosition();
		void setPosition(XMFLOAT3 position);

		Colliders* getCollider();
		void SetNode(CollisionNode* colNode);
		virtual CollisionNode* getNode() override;

		virtual void BuildBoundingBox(XMFLOAT3 radius);
		void BuildOrientedBoundingBox(XMFLOAT3 radius, XMFLOAT4 orientation);

		virtual void CheckTriggers();
		void setIsSelected(bool value);
		bool getIsSelected();


		bool mIsSelected = false;
		bool mIsEdited = false;

		void StartMoving(std::vector<XMFLOAT2> positions);
		void SetPathFindingMoveFlag(bool value);
		bool GetPathFindingMoveFlag();

		void SetLightsReferences(std::vector<DirectionalLight*>& directionalLights, std::vector<PointLight*>& pointLights, std::vector<SpotLight*>& spotLights);
		const char* GetName();

	protected:
		GameObject();
		GameObject(const GameObject& rhs);
		GameObject& operator=(const GameObject& rhs);

		void Move();
		void ChangeTexture(std::string textureName);

		Effect* mEffect;

		XMFLOAT3 mPosition;
		XMFLOAT3 mRotation;
		XMFLOAT3 mScale;

		std::string mIsSelectedDiffuseMap = "";
		std::string mIsBusyDiffuseMap = "";

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
		std::vector<ID3D11ShaderResourceView*> mNormalTextures;
		std::vector<ID3D11ShaderResourceView*> mSpecularTextures;
		Model* mModel;
		AnimationPlayer* mAnimationPlayer;

		RenderStateHelper mRenderStateHelper;
		SpriteBatch* mSpriteBatch;
		SpriteFont* mSpriteFont;
		XMFLOAT2 mTextPosition;
		bool mManualAdvanceMode;	

		Colliders* mCollider;
		CollisionNode* inNode;

		bool mustBeDestroyed;
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

		private:
			void StandarizeRotationVectorValue();
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
