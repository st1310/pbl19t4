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
			const char *modelName, LPCWSTR shaderName, std::string diffuseMap,
			XMFLOAT3 startPosition, XMFLOAT3 startRotation, XMFLOAT3 startScale)
		: DrawableGameComponent(game, camera),
		mEffect(nullptr), mWorldMatrix(MatrixHelper::Identity),
		mClassName(className),
		mVertexBuffers(), mIndexBuffers(), mIndexCounts(), mColorTextures(),
		mShaderName(shaderName), mModelName(modelName), mDiffuseMap(diffuseMap),
		mSkinnedModel(nullptr), mAnimationPlayer(nullptr),
		mRenderStateHelper(game), mSpriteBatch(nullptr), mSpriteFont(nullptr), mTextPosition(0.0f, 400.0f), mManualAdvanceMode(false),
		mKeyboard(nullptr),
		mPosition(startPosition), mRotation(startRotation), mScale(startScale)
	{
	}

	GameObject::~GameObject()
	{
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
		XMMATRIX transformX = XMMatrixRotationX(XMConvertToRadians(x));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformX);

		XMMATRIX transformY = XMMatrixRotationY(XMConvertToRadians(y));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformY);

		XMMATRIX transformZ = XMMatrixRotationZ(XMConvertToRadians(z));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformZ);
	}

	void GameObject::Rotate(XMFLOAT3 rotation)
	{
		mRotation.x += rotation.x;
		mRotation.y += rotation.y;
		mRotation.z += rotation.z;

		XMMATRIX transformX = XMMatrixRotationX(XMConvertToRadians(rotation.x));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformX);

		XMMATRIX transformY = XMMatrixRotationY(XMConvertToRadians(rotation.y));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformY);

		XMMATRIX transformZ = XMMatrixRotationZ(XMConvertToRadians(rotation.z));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformZ);
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

	void GameObject::Translate(float x, float y, float z)
	{
		mPosition.x += x;
		mPosition.y += y;
		mPosition.z += z;
		XMMATRIX translate = XMMatrixTranslation(x, y, z);
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * translate);
	}

	void GameObject::Translate(XMFLOAT3 translation)
	{
		XMMATRIX translate = XMMatrixTranslation(translation.x, translation.y, translation.z);
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * translate);
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
			mEditFactor += 0.05;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_NUMPADMINUS))
			mEditFactor -= 0.05;

		if (mEditFactor < 0)
			mEditFactor = 0;
	}

	void GameObject::SetPosition()
	{
		if (mPrecisionMode)
		{
			if (mEditAxis == X_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					Translate(mEditFactor, 0, 0);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					Translate(-mEditFactor, 0, 0);
			}
			if (mEditAxis == Y_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					Translate(0, mEditFactor, 0);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					Translate(0, -mEditFactor, 0);
			}
			if (mEditAxis == Z_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					Translate(0, 0, mEditFactor);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					Translate(0, 0, -mEditFactor);
			}
		}
		else
		{
			if (mEditAxis == X_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					Translate(mEditFactor, 0, 0);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					Translate(-mEditFactor, 0, 0);
			}
			if (mEditAxis == Y_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					Translate(0, mEditFactor, 0);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					Translate(0, -mEditFactor, 0);
			}
			if (mEditAxis == Z_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					Translate(0, 0, mEditFactor);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					Translate(0, 0, -mEditFactor);
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
					Rotate(mEditFactor, 0, 0);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					Rotate(-mEditFactor, 0, 0);
			}
			if (mEditAxis == Y_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					Rotate(0, mEditFactor, 0);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					Rotate(0, -mEditFactor, 0);
			}
			if (mEditAxis == Z_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					Rotate(0, 0, mEditFactor);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					Rotate(0, 0, -mEditFactor);
			}
		}

		else
		{
			if (mEditAxis == X_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					Rotate(mEditFactor, 0, 0);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					Rotate(-mEditFactor, 0, 0);
			}
			if (mEditAxis == Y_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					Rotate(0, mEditFactor, 0);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					Rotate(0, -mEditFactor, 0);
			}
			if (mEditAxis == Z_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					Rotate(0, 0, mEditFactor);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					Rotate(0, 0, -mEditFactor);
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
					Scale(mEditFactor, 0, 0);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					Scale(-mEditFactor, 0, 0);
			}
			if (mEditAxis == Y_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					Scale(0, mEditFactor, 0);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					Scale(0, -mEditFactor, 0);
			}
			if (mEditAxis == Z_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					Scale(0, 0, mEditFactor);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					Scale(0, 0, -mEditFactor);
			}
			if (mEditAxis == ALL_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					Scale(mEditFactor, mEditFactor, mEditFactor);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					Scale(-mEditFactor, -mEditFactor, -mEditFactor);
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
					Scale(mEditFactor, 0, 0);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					Scale(-mEditFactor, 0, 0);
			}
			if (mEditAxis == Y_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					Scale(0, mEditFactor, 0);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					Scale(0, -mEditFactor, 0);
			}
			if (mEditAxis == Z_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					Scale(0, 0, mEditFactor);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					Scale(0, 0, -mEditFactor);
			}
			if (mEditAxis == ALL_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					Scale(mEditFactor, mEditFactor, mEditFactor);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					Scale(-mEditFactor, -mEditFactor, -mEditFactor);
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
		}


		if (mKeyboard->WasKeyPressedThisFrame(DIK_NUMPAD2))
		{
			mAxisNumber = X_AXIS_NUMBER;
			mEditMode = ROTATION;
		}


		if (mKeyboard->WasKeyPressedThisFrame(DIK_NUMPAD3))
		{
			mAxisNumber = X_AXIS_NUMBER;
			mEditMode = SCALE;
		}


		if (mKeyboard->WasKeyPressedThisFrame(DIK_NUMPAD4))
			mPrecisionMode = !mPrecisionMode;
	}

	std::wostringstream GameObject::GetCreationKitInfo()
	{
		std::wostringstream helpLabel;
		std::string precisionSwitch = (mPrecisionMode == true) ? "On" : "Off";

		helpLabel << "\nMode: " << mEditMode.c_str() <<
			" | Axis: " << mEditAxis.c_str() <<
			" | Edit factor: " << mEditFactor << " | Precision mode: " << precisionSwitch.c_str();
		helpLabel << "\nPosition: " << mPosition.x << ", " << mPosition.y << ", " << mPosition.z;
		helpLabel << "\nRotation: " << mRotation.x << ", " << mRotation.y << ", " << mRotation.z;
		helpLabel << "\nScale: " << mScale.x << ", " << mScale.y << ", " << mScale.z;

		return helpLabel;
	}

}