#include "GameObject.h"
#include "Game.h"
#include "GameException.h"
#include "MatrixHelper.h"
#include "Camera.h"
#include "Utility.h"
#include "../Library/Model.h"
#include "Mesh.h"
#include "ModelMaterial.h"
#include "KeyboardComponent.h"
#include "SkinnedModelMaterial.h"
#include "VectorHelper.h"
#include "ColorHelper.h"
#include "AnimationPlayer.h"
#include "AnimationClip.h"
#include <WICTextureLoader.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <sstream>
#include <iomanip>
#include "Shlwapi.h"

namespace Rendering
{
	RTTI_DEFINITIONS(GameObject)

		GameObject::GameObject(Game& game, Camera& camera, const char *className,
			LPCWSTR shaderName,
			XMFLOAT3 startPosition, XMFLOAT3 startRotation, XMFLOAT3 startScale)
		: DrawableGameComponent(game, camera),
		mEffect(nullptr), mWorldMatrix(MatrixHelper::Identity),
		mClassName(className),
		mVertexBuffers(), mIndexBuffers(), mIndexCounts(), mColorTextures(),
		mShaderName(shaderName),
		mSkinnedModel(nullptr), mAnimationPlayer(nullptr),
		mRenderStateHelper(game), mSpriteBatch(nullptr), mSpriteFont(nullptr), mTextPosition(0.0f, 400.0f), mManualAdvanceMode(false),
		mKeyboard(nullptr),
		mPosition(startPosition), mRotation(startRotation), mScale(startScale), mCollider(), inNode(nullptr), 
		mustBeDestroyed(false), mState(new State()), objectToMove(false)
	{
	}

	GameObject::~GameObject()
	{
		mCollider = nullptr;
		inNode = nullptr;
	}

	bool GameObject::IsMustBeDestroyed()
	{
		return mustBeDestroyed;
	}

	void GameObject::Initialize()
	{
	}

	void GameObject::Update(const GameTime& gameTime)
	{
	}

	void GameObject::Draw(const GameTime& gameTime)
	{
	}

	void GameObject::ChangeTexture(std::string diffuseMap)
	{
		if (diffuseMap == "")
			return;

		ID3D11ShaderResourceView* colorTexture = nullptr;
		std::wostringstream textureName;
		textureName << diffuseMap.c_str();

		HRESULT hr = DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), textureName.str().c_str(), nullptr, &colorTexture);

		if (FAILED(hr))
			throw GameException("CreateWICTextureFromFile() failed.", hr);

		mColorTextures[0] = colorTexture;
	}


	// Transformation
	void GameObject::Scale(float x, float y, float z)
	{
		XMFLOAT3 newScale = XMFLOAT3(
			mScale.x + x,
			mScale.y + y,
			mScale.z + z
		);
		mScale = newScale;
		XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(mScale.x, mScale.y, mScale.z));
	}

	void GameObject::Scale(XMFLOAT3 scale)
	{
		XMFLOAT3 newScale = XMFLOAT3(
			mScale.x + scale.x,
			mScale.y + scale.y,
			mScale.z + scale.z
		);
		mScale = newScale;
		XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(mScale.x, mScale.y, mScale.z));
	}

	void GameObject::Rotate(float x, float y, float z)
	{
		mRotation.x += x;
		mRotation.y += y;
		mRotation.z += z;

		StandarizeRotationVectorValue();

		XMFLOAT3 originalTransation = XMFLOAT3(-mOriginalPosition.x, -mOriginalPosition.y, -mOriginalPosition.z);
		Translate(originalTransation);

		XMMATRIX transformX = XMMatrixRotationX(XMConvertToRadians(x));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformX);

		XMMATRIX transformY = XMMatrixRotationY(XMConvertToRadians(y));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformY);

		XMMATRIX transformZ = XMMatrixRotationZ(XMConvertToRadians(z));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformZ);

		Translate(XMFLOAT3(-originalTransation.x, -originalTransation.y, -originalTransation.z));
	}

	void GameObject::Rotate(XMFLOAT3 rotation)
	{
		Rotate(rotation.x, rotation.y, rotation.z);
	}

	void GameObject::FirstRotation()
	{
		XMMATRIX transformX = XMMatrixRotationX(XMConvertToRadians(mRotation.x));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformX);

		XMMATRIX transformY = XMMatrixRotationY(XMConvertToRadians(mRotation.y));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformY);

		XMMATRIX transformZ = XMMatrixRotationZ(XMConvertToRadians(mRotation.z));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformZ);
	}

	void GameObject::FirstTranslation(XMFLOAT3 translation)
	{
		mOriginalPosition.x += translation.x;
		mOriginalPosition.y += translation.y;
		mOriginalPosition.z += translation.z;

		XMMATRIX translate = XMMatrixTranslation(translation.x, translation.y, translation.z);
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * translate);
	}

	void GameObject::Translate(float x, float y, float z)
	{
		mPosition.x += x;
		mPosition.y += y;
		mPosition.z += z;

		mOriginalPosition.x += x;
		mOriginalPosition.y += y;
		mOriginalPosition.z += z;

		XMMATRIX translate = XMMatrixTranslation(x, y, z);
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * translate);
	}

	void GameObject::Translate(XMFLOAT3 translation)
	{
		Translate(translation.x, translation.y, translation.z);
	}

	void GameObject::StandarizeRotationVectorValue()
	{
		// X
		if (mRotation.x >= 360)
			mRotation.x -= 360;

		if (mRotation.x <= -360)
			mRotation.x += 360;

		// Y
		if (mRotation.y >= 360)
			mRotation.y -= 360;

		if (mRotation.y <= -360)
			mRotation.y += 360;

		// Z
		if (mRotation.z >= 360)
			mRotation.z -= 360;

		if (mRotation.z <= -360)
			mRotation.z += 360;
	}

	// Creation Kit
	std::vector<std::string> GameObject::Serialize()
	{
		std::vector<std::string> result = std::vector<std::string>();

		result.push_back(mClassName);

		// Position
		result.push_back(std::to_string(mPosition.x));
		result.push_back(std::to_string(mPosition.y));
		result.push_back(std::to_string(mPosition.z));

		// Rotation
		result.push_back(std::to_string(mRotation.x));
		result.push_back(std::to_string(mRotation.y));
		result.push_back(std::to_string(mRotation.z));

		// Scale
		result.push_back(std::to_string(mScale.x));
		result.push_back(std::to_string(mScale.y));
		result.push_back(std::to_string(mScale.z));

		return result;
	}

	XMFLOAT3 GameObject::getPosition()
	{
		return mPosition;
	}

	void GameObject::EditModel()
	{
		ChangeEditFactor();
		ChangeEditMode();
		ChangeEditAxis();

		if (mEditMode == POSITION)
			SetPosition();

		if (mEditMode == ROTATION)
			SetRotation();

		if (mEditMode == SCALE)
			SetScale();
	}

	void GameObject::ChangeEditFactor()
	{
		if (mKeyboard->WasKeyPressedThisFrame(DIK_NUMPADPLUS))
			mEditFactorIndex++;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_NUMPADMINUS))
			mEditFactorIndex--;

		if (mEditFactorIndex < 0)
			mEditFactorIndex = 0;

		if (mEditFactorIndex >= 11)
			mEditFactorIndex = 10;
	}

	void GameObject::SetPosition()
	{
		if (mPrecisionMode)
		{
			if (mXYPositionMode)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_RIGHTARROW))
					Translate(mEditFactorArray[mEditFactorIndex], 0, 0);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_LEFTARROW))
					Translate(-mEditFactorArray[mEditFactorIndex], 0, 0);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					Translate(0, 0, mEditFactorArray[mEditFactorIndex]);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					Translate(0, 0, -mEditFactorArray[mEditFactorIndex]);
			}
			else
			{
				if (mEditAxis == X_AXIS)
				{
					if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
						Translate(mEditFactorArray[mEditFactorIndex], 0, 0);

					if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
						Translate(-mEditFactorArray[mEditFactorIndex], 0, 0);
				}
				if (mEditAxis == Y_AXIS)
				{
					if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
						Translate(0, mEditFactorArray[mEditFactorIndex], 0);

					if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
						Translate(0, -mEditFactorArray[mEditFactorIndex], 0);
				}
				if (mEditAxis == Z_AXIS)
				{
					if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
						Translate(0, 0, mEditFactorArray[mEditFactorIndex]);

					if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
						Translate(0, 0, -mEditFactorArray[mEditFactorIndex]);
				}
			}
		}
		else
		{
			if (mXYPositionMode)
			{
				if (mKeyboard->IsKeyDown(DIK_RIGHTARROW))
					Translate(mEditFactorArray[mEditFactorIndex], 0, 0);

				if (mKeyboard->IsKeyDown(DIK_LEFTARROW))
					Translate(-mEditFactorArray[mEditFactorIndex], 0, 0);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					Translate(0, 0, mEditFactorArray[mEditFactorIndex]);

				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					Translate(0, 0, -mEditFactorArray[mEditFactorIndex]);
			}
			else
			{
				if (mEditAxis == X_AXIS)
				{
					if (mKeyboard->IsKeyDown(DIK_UPARROW))
						Translate(mEditFactorArray[mEditFactorIndex], 0, 0);

					if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
						Translate(-mEditFactorArray[mEditFactorIndex], 0, 0);
				}
				if (mEditAxis == Y_AXIS)
				{
					if (mKeyboard->IsKeyDown(DIK_UPARROW))
						Translate(0, mEditFactorArray[mEditFactorIndex], 0);

					if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
						Translate(0, -mEditFactorArray[mEditFactorIndex], 0);
				}
				if (mEditAxis == Z_AXIS)
				{
					if (mKeyboard->IsKeyDown(DIK_UPARROW))
						Translate(0, 0, mEditFactorArray[mEditFactorIndex]);

					if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
						Translate(0, 0, -mEditFactorArray[mEditFactorIndex]);
				}
			}
		}
	}

	void GameObject::SetRotation()
	{
		if (mPrecisionMode)
		{
			if (mEditAxis == X_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					Rotate(mEditFactorArray[mEditFactorIndex], 0, 0);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					Rotate(-mEditFactorArray[mEditFactorIndex], 0, 0);
			}
			if (mEditAxis == Y_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					Rotate(0, mEditFactorArray[mEditFactorIndex], 0);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					Rotate(0, -mEditFactorArray[mEditFactorIndex], 0);
			}
			if (mEditAxis == Z_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					Rotate(0, 0, mEditFactorArray[mEditFactorIndex]);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					Rotate(0, 0, -mEditFactorArray[mEditFactorIndex]);
			}
		}

		else
		{
			if (mEditAxis == X_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					Rotate(mEditFactorArray[mEditFactorIndex], 0, 0);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					Rotate(-mEditFactorArray[mEditFactorIndex], 0, 0);
			}
			if (mEditAxis == Y_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					Rotate(0, mEditFactorArray[mEditFactorIndex], 0);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					Rotate(0, -mEditFactorArray[mEditFactorIndex], 0);
			}
			if (mEditAxis == Z_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					Rotate(0, 0, mEditFactorArray[mEditFactorIndex]);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					Rotate(0, 0, -mEditFactorArray[mEditFactorIndex]);
			}
		}

	}

	void GameObject::SetScale()
	{
		if (mPrecisionMode)
		{
			if (mEditAxis == X_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					Scale(mEditFactorArray[mEditFactorIndex], 0, 0);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					Scale(-mEditFactorArray[mEditFactorIndex], 0, 0);
			}
			if (mEditAxis == Y_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					Scale(0, mEditFactorArray[mEditFactorIndex], 0);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					Scale(0, -mEditFactorArray[mEditFactorIndex], 0);
			}
			if (mEditAxis == Z_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					Scale(0, 0, mEditFactorArray[mEditFactorIndex]);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					Scale(0, 0, -mEditFactorArray[mEditFactorIndex]);
			}
			if (mEditAxis == ALL_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					Scale(mEditFactorArray[mEditFactorIndex], mEditFactorArray[mEditFactorIndex], mEditFactorArray[mEditFactorIndex]);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					Scale(-mEditFactorArray[mEditFactorIndex], -mEditFactorArray[mEditFactorIndex], -mEditFactorArray[mEditFactorIndex]);
			}

			// Fix rotation after scaling
			if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW) || mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
				FirstRotation();
		}

		else
		{
			if (mEditAxis == X_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					Scale(mEditFactorArray[mEditFactorIndex], 0, 0);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					Scale(-mEditFactorArray[mEditFactorIndex], 0, 0);
			}
			if (mEditAxis == Y_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					Scale(0, mEditFactorArray[mEditFactorIndex], 0);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					Scale(0, -mEditFactorArray[mEditFactorIndex], 0);
			}
			if (mEditAxis == Z_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					Scale(0, 0, mEditFactorArray[mEditFactorIndex]);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					Scale(0, 0, -mEditFactorArray[mEditFactorIndex]);
			}
			if (mEditAxis == ALL_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					Scale(mEditFactorArray[mEditFactorIndex], mEditFactorArray[mEditFactorIndex], mEditFactorArray[mEditFactorIndex]);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					Scale(-mEditFactorArray[mEditFactorIndex], -mEditFactorArray[mEditFactorIndex], -mEditFactorArray[mEditFactorIndex]);
			}

			// Fix rotation after scaling
			if (mKeyboard->IsKeyDown(DIK_UPARROW) || mKeyboard->IsKeyDown(DIK_DOWNARROW))
				FirstRotation();
		}
	}

	void GameObject::ChangeEditAxis()
	{
		if (mKeyboard->WasKeyPressedThisFrame(DIK_LEFTARROW))
			mAxisNumber--;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_RIGHTARROW))
			mAxisNumber++;

		if (mEditMode == SCALE)
		{
			if (mAxisNumber < X_AXIS_NUMBER)
				mAxisNumber = ALL_AXIS_NUMBER;

			if (mAxisNumber > ALL_AXIS_NUMBER)
				mAxisNumber = X_AXIS_NUMBER;
		}

		else
		{
			if (mAxisNumber < X_AXIS_NUMBER)
				mAxisNumber = Z_AXIS_NUMBER;

			if (mAxisNumber > Z_AXIS_NUMBER)
				mAxisNumber = X_AXIS_NUMBER;
		}

		if (mAxisNumber == X_AXIS_NUMBER)
			mEditAxis = X_AXIS;

		if (mAxisNumber == Y_AXIS_NUMBER)
			mEditAxis = Y_AXIS;

		if (mAxisNumber == Z_AXIS_NUMBER)
			mEditAxis = Z_AXIS;

		if (mAxisNumber == ALL_AXIS_NUMBER)
			mEditAxis = ALL_AXIS;
	}

	void GameObject::ChangeEditMode()
	{
		if (mKeyboard->WasKeyPressedThisFrame(DIK_NUMPAD1))
		{
			mAxisNumber = X_AXIS_NUMBER;
			mEditMode = POSITION;
			mEditFactorIndex = 4;
			mPrecisionMode = false;
			mXYPositionMode = true;
		}


		if (mKeyboard->WasKeyPressedThisFrame(DIK_NUMPAD2))
		{
			mAxisNumber = X_AXIS_NUMBER;
			mEditMode = ROTATION;
			mEditFactorIndex = 5;
			mPrecisionMode = false;
		}


		if (mKeyboard->WasKeyPressedThisFrame(DIK_NUMPAD3))
		{
			mAxisNumber = ALL_AXIS_NUMBER;
			mEditMode = SCALE;
			mEditFactorIndex = 1;
			mPrecisionMode = true;
		}


		if (mKeyboard->WasKeyPressedThisFrame(DIK_NUMPAD4))
			mPrecisionMode = !mPrecisionMode;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_NUMPAD5))
			mXYPositionMode = !mXYPositionMode;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_NUMPAD7))
			mEditFactorIndex = 0;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_NUMPAD9))
			mEditFactorIndex = 10;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_X))
		{
			mIsSelected = false;
			mIsEdited = false;
		}
	}

	std::wostringstream GameObject::GetCreationKitInfo()
	{
		std::wostringstream helpLabel;

		std::string precisionSwitch = (mPrecisionMode == true) ? "On" : "Off";
		std::string positionXYSwitch = (mXYPositionMode == true) ? "XY Only" : "Normal";
		std::string currentAxis = (mEditMode == POSITION && mXYPositionMode == true) ? "XY" : mEditAxis;

		helpLabel << "\nMode: " << mEditMode.c_str() <<
			" | Axis: " << currentAxis.c_str() <<
			" | Edit factor: " << mEditFactorArray[mEditFactorIndex] << " | Precision mode: " << precisionSwitch.c_str();

		if (mEditMode == POSITION)
			helpLabel << " | Position mode: " << positionXYSwitch.c_str();

		helpLabel << "\nPosition: " << mPosition.x << ", " << mPosition.y << ", " << mPosition.z;
		helpLabel << "\nRotation: " << mRotation.x << ", " << mRotation.y << ", " << mRotation.z;
		helpLabel << "\nScale: " << mScale.x << ", " << mScale.y << ", " << mScale.z;

		return helpLabel;
	}

	Colliders* GameObject::getCollider()
	{
		return mCollider;
	}

	void  GameObject::SetNode(CollisionNode* colNode)
	{
		inNode = colNode;
	}

	CollisionNode*  GameObject::getNode()
	{
		return inNode;
	}

	void GameObject::setIsSelected(bool value) {
		mIsSelected = value;
	}

	bool GameObject::getIsSelected() {
		return mIsSelected;
	}

	void GameObject::BuildBoundingBox(XMFLOAT3 radius)
	{
	}

	void GameObject::BuildOrientedBoundingBox(XMFLOAT3 radius, XMFLOAT4 orientation)
	{
		XMVECTOR helper;
		helper = XMVector3Transform(XMLoadFloat3(&radius), XMLoadFloat4x4(&mWorldMatrix));
		XMStoreFloat3(&radius, helper);
		mCollider->BuildOBB(mPosition, radius, orientation);

		if (inNode != nullptr)
			inNode->AddStaticCollider(mCollider);
	}

	void GameObject::CheckTriggers()
	{}

	void GameObject::Move()
	{
		Translate(mState->Move());
		Rotate(mState->Rotate());

	}

	void GameObject::StartMoving(std::vector<XMFLOAT2> positions)
	{
		XMFLOAT2 currentPosition = XMFLOAT2(mPosition.x, mPosition.z);

		mState->MoveInit(currentPosition, positions, mRotation.y, mTranslationSpeed*2, mRotationSpeed, mCollider, inNode);

		ChangeTexture(mIsBusyDiffuseMap);
		mIsBusy = true;
	}

	void GameObject::SetPathFindingMoveFlag(bool value) {
		objectToMove = value;
	}

	bool GameObject::GetPathFindingMoveFlag() {
		return objectToMove;
	}
}