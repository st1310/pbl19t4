﻿#include "GameObject.h"
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

		GameObject::GameObject(Game& game, Camera& camera, const char *className, const char *modelName, LPCWSTR shaderName, XMFLOAT3 startPosition, XMFLOAT3 startRotation, XMFLOAT3 startScale)
		: DrawableGameComponent(game, camera),
		mMaterial(nullptr), mEffect(nullptr), mWorldMatrix(MatrixHelper::Identity),
		mVertexBuffers(), mIndexBuffers(), mIndexCounts(), mColorTextures(),
		mKeyboard(nullptr),
		mClassName(className),
		mShaderName(shaderName), mModelName(modelName), 
		mPosition(startPosition), mRotation(startRotation), mScale(startScale),
		mSkinnedModel(nullptr), mAnimationPlayer(nullptr),
		mRenderStateHelper(game), mSpriteBatch(nullptr), mSpriteFont(nullptr), mTextPosition(0.0f, 400.0f), mManualAdvanceMode(false)
	{		
	}

	GameObject::~GameObject()
	{
		for (ID3D11Buffer* vertexBuffer : mVertexBuffers)
		{
			ReleaseObject(vertexBuffer);
		}

		for (ID3D11Buffer* indexBuffer : mIndexBuffers)
		{
			ReleaseObject(indexBuffer);
		}

		for (ID3D11ShaderResourceView* colorTexture : mColorTextures)
		{
			ReleaseObject(colorTexture);
		}

		DeleteObject(mSpriteFont);
		DeleteObject(mSpriteBatch);
		DeleteObject(mSkinnedModel);
		DeleteObject(mAnimationPlayer);
		DeleteObject(mMaterial);
		DeleteObject(mEffect);
	}

	void GameObject::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		// Load the model
		mSkinnedModel = new Model(*mGame, mModelName, true);

		// Initialize the material
		mEffect = new Effect(*mGame);
		mEffect->LoadCompiledEffect(mShaderName);

		mMaterial = new SkinnedModelMaterial();
		mMaterial->Initialize(mEffect);

		// Create the vertex and index buffers
		mVertexBuffers.resize(mSkinnedModel->Meshes().size());
		mIndexBuffers.resize(mSkinnedModel->Meshes().size());
		mIndexCounts.resize(mSkinnedModel->Meshes().size());
		mColorTextures.resize(mSkinnedModel->Meshes().size());
		for (UINT i = 0; i < mSkinnedModel->Meshes().size(); i++)
		{
			Mesh* mesh = mSkinnedModel->Meshes().at(i);

			ID3D11Buffer* vertexBuffer = nullptr;
			mMaterial->CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, &vertexBuffer);
			mVertexBuffers[i] = vertexBuffer;

			ID3D11Buffer* indexBuffer = nullptr;
			mesh->CreateIndexBuffer(&indexBuffer);
			mIndexBuffers[i] = indexBuffer;

			mIndexCounts[i] = mesh->Indices().size();

			ID3D11ShaderResourceView* colorTexture = nullptr;
			ModelMaterial* material = mesh->GetMaterial();

			if (material != nullptr && material->Textures().find(TextureTypeDifffuse) != material->Textures().cend())
			{
				std::vector<std::wstring>* diffuseTextures = material->Textures().at(TextureTypeDifffuse);
				std::wstring filename = PathFindFileName(diffuseTextures->at(0).c_str());

				std::wostringstream textureName;
				textureName << L"Content\\Models\\" << filename;
				HRESULT hr = DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), textureName.str().c_str(), nullptr, &colorTexture);
				if (FAILED(hr))
				{
					throw GameException("CreateWICTextureFromFile() failed.", hr);
				}
			}
			mColorTextures[i] = colorTexture;
		}

		mKeyboard = (KeyboardComponent*)mGame->Services().GetService(KeyboardComponent::TypeIdClass());
		assert(mKeyboard != nullptr);

		mAnimationPlayer = new AnimationPlayer(*mGame, *mSkinnedModel, false);
		mAnimationPlayer->StartClip(*(mSkinnedModel->Animations().at(0)));

		mSpriteBatch = new SpriteBatch(mGame->Direct3DDeviceContext());
		mSpriteFont = new SpriteFont(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");

		// Initial transform
		
		GameObject::Scale(0,0,0);
		GameObject::FirstRotation();
		GameObject::Translate(mPosition);
			
	}

	void GameObject::Update(const GameTime& gameTime)
	{
		UpdateOptions();

		if (mManualAdvanceMode == false)
		{
			mAnimationPlayer->Update(gameTime);
		}
	}

	void GameObject::Draw(const GameTime& gameTime)
	{
		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Pass* pass = mMaterial->CurrentTechnique()->Passes().at(0);
		ID3D11InputLayout* inputLayout = mMaterial->InputLayouts().at(pass);
		direct3DDeviceContext->IASetInputLayout(inputLayout);

		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX wvp = worldMatrix * mCamera->ViewMatrix() * mCamera->ProjectionMatrix();

		UINT stride = mMaterial->VertexSize();
		UINT offset = 0;

		for (UINT i = 0; i < mVertexBuffers.size(); i++)
		{
			ID3D11Buffer* vertexBuffer = mVertexBuffers[i];
			ID3D11Buffer* indexBuffer = mIndexBuffers[i];
			UINT indexCount = mIndexCounts[i];
			ID3D11ShaderResourceView* colorTexture = mColorTextures[i];

			direct3DDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
			direct3DDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

			mMaterial->WorldViewProjection() << wvp;
			mMaterial->ColorTexture() << colorTexture;
			mMaterial->BoneTransforms() << mAnimationPlayer->BoneTransforms();

			pass->Apply(0, direct3DDeviceContext);

			direct3DDeviceContext->DrawIndexed(indexCount, 0, 0);
		}

		mRenderStateHelper.SaveAll();
		mSpriteBatch->Begin();

		std::wostringstream helpLabel;
		
		helpLabel << std::setprecision(5) << L"\nAnimation Time: " << mAnimationPlayer->CurrentTime()
			<< "\nFrame Interpolation (I): " << (mAnimationPlayer->InterpolationEnabled() ? "On" : "Off");
		
		if (mManualAdvanceMode)
		{
			helpLabel << "\nCurrent Keyframe (Space): " << mAnimationPlayer->CurrentKeyframe();
		}
		else
		{
			helpLabel << "\nPause / Resume(P)";
		}
		if (mIsSelected && mIsEdited)
		{
			helpLabel << "\nMode: " << mEditMode.at(0) << mEditMode.at(1) << mEditMode.at(2) <<
				" | Axis: " << mEditAxis.at(0) <<
				" | Edit factor: " << mEditFactor << " | Precision mode: " << mPrecisionMode;
			helpLabel << "\nPosition: " << mPosition.x << ", " << mPosition.y << ", " << mPosition.z;
			helpLabel << "\nRotation: " << mRotation.x << ", " << mRotation.y << ", " << mRotation.z;
			helpLabel << "\nScale: " << mScale.x << ", " << mScale.y << ", " << mScale.z;
		}

		mSpriteFont->DrawString(mSpriteBatch, helpLabel.str().c_str(), mTextPosition);

		mSpriteBatch->End();
		mRenderStateHelper.RestoreAll();
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

	void GameObject::UpdateOptions()
	{
		if (mKeyboard != nullptr)
		{
			if (mKeyboard->WasKeyPressedThisFrame(DIK_U))
			{
				//
			}

			if (mKeyboard->WasKeyPressedThisFrame(DIK_P))
			{
				if (mAnimationPlayer->IsPlayingClip())
				{
					mAnimationPlayer->PauseClip();
				}
				else
				{
					mAnimationPlayer->ResumeClip();
				}
			}

			if (mKeyboard->WasKeyPressedThisFrame(DIK_B))
			{
				// Reset the animation clip to the bind pose
				mAnimationPlayer->StartClip(*(mSkinnedModel->Animations().at(0)));
			}

			if (mKeyboard->WasKeyPressedThisFrame(DIK_I))
			{
				// Enable/disabled interpolation
				mAnimationPlayer->SetInterpolationEnabled(!mAnimationPlayer->InterpolationEnabled());
			}

			if (mKeyboard->WasKeyPressedThisFrame(DIK_RETURN))
			{
				// Enable/disable manual advance mode
				mManualAdvanceMode = !mManualAdvanceMode;
				mAnimationPlayer->SetCurrentKeyFrame(0);
			}

			if (mManualAdvanceMode && mKeyboard->WasKeyPressedThisFrame(DIK_SPACE))
			{
				// Advance the current keyframe
				UINT currentKeyFrame = mAnimationPlayer->CurrentKeyframe();
				currentKeyFrame++;
				if (currentKeyFrame >= mAnimationPlayer->CurrentClip()->KeyframeCount())
				{
					currentKeyFrame = 0;
				}

				mAnimationPlayer->SetCurrentKeyFrame(currentKeyFrame);
			}

			if(mIsSelected && mIsEdited)
				EditModel();
		}
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
					GameObject::Translate(mEditFactor, 0, 0);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					GameObject::Translate(-mEditFactor, 0, 0);
			}
			if (mEditAxis == Y_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					GameObject::Translate(0, mEditFactor, 0);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					GameObject::Translate(0, -mEditFactor, 0);
			}
			if (mEditAxis == Z_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					GameObject::Translate(0, 0, mEditFactor);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					GameObject::Translate(0, 0, -mEditFactor);
			}
		}
		else
		{
			if (mEditAxis == X_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					GameObject::Translate(mEditFactor, 0, 0);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					GameObject::Translate(-mEditFactor, 0, 0);
			}
			if (mEditAxis == Y_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					GameObject::Translate(0, mEditFactor, 0);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					GameObject::Translate(0, -mEditFactor, 0);
			}
			if (mEditAxis == Z_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					GameObject::Translate(0, 0, mEditFactor);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					GameObject::Translate(0, 0, -mEditFactor);
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
					GameObject::Rotate(mEditFactor, 0, 0);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					GameObject::Rotate(-mEditFactor, 0, 0);
			}
			if (mEditAxis == Y_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					GameObject::Rotate(0, mEditFactor, 0);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					GameObject::Rotate(0, -mEditFactor, 0);
			}
			if (mEditAxis == Z_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					GameObject::Rotate(0, 0, mEditFactor);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					GameObject::Rotate(0, 0, -mEditFactor);
			}
		}

		else
		{
			if (mEditAxis == X_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					GameObject::Rotate(mEditFactor, 0, 0);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					GameObject::Rotate(-mEditFactor, 0, 0);
			}
			if (mEditAxis == Y_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					GameObject::Rotate(0, mEditFactor, 0);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					GameObject::Rotate(0, -mEditFactor, 0);
			}
			if (mEditAxis == Z_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					GameObject::Rotate(0, 0, mEditFactor);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					GameObject::Rotate(0, 0, -mEditFactor);
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
					GameObject::Scale(mEditFactor, 0, 0);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					GameObject::Scale(-mEditFactor, 0, 0);
			}
			if (mEditAxis == Y_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					GameObject::Scale(0, mEditFactor, 0);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					GameObject::Scale(0, -mEditFactor, 0);
			}
			if (mEditAxis == Z_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					GameObject::Scale(0, 0, mEditFactor);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					GameObject::Scale(0, 0, -mEditFactor);
			}
			if (mEditAxis == ALL_AXIS)
			{
				if (mKeyboard->WasKeyPressedThisFrame(DIK_UPARROW))
					GameObject::Scale(mEditFactor, mEditFactor, mEditFactor);

				if (mKeyboard->WasKeyPressedThisFrame(DIK_DOWNARROW))
					GameObject::Scale(-mEditFactor, -mEditFactor, -mEditFactor);
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
					GameObject::Scale(mEditFactor, 0, 0);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					GameObject::Scale(-mEditFactor, 0, 0);
			}
			if (mEditAxis == Y_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					GameObject::Scale(0, mEditFactor, 0);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					GameObject::Scale(0, -mEditFactor, 0);
			}
			if (mEditAxis == Z_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					GameObject::Scale(0, 0, mEditFactor);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					GameObject::Scale(0, 0, -mEditFactor);
			}
			if (mEditAxis == ALL_AXIS)
			{
				if (mKeyboard->IsKeyDown(DIK_UPARROW))
					GameObject::Scale(mEditFactor, mEditFactor, mEditFactor);

				if (mKeyboard->IsKeyDown(DIK_DOWNARROW))
					GameObject::Scale(-mEditFactor, -mEditFactor, -mEditFactor);
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

}