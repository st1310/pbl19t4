#include "AnimatedGameObject.h"
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
#include "Colliders.h"
#include "NodeLIst.h"

namespace Rendering
{
	RTTI_DEFINITIONS(AnimatedGameObject)

		AnimatedGameObject::AnimatedGameObject(Game& game, Camera& camera, const char *className,
			const char *modelName, LPCWSTR shaderName, std::string diffuseMap,
			XMFLOAT3 startPosition, XMFLOAT3 startRotation, XMFLOAT3 startScale)
		: DrawableGameComponent(game, camera),
		mMaterial(nullptr), mEffect(nullptr), mWorldMatrix(MatrixHelper::Identity),
		mVertexBuffers(), mIndexBuffers(), mIndexCounts(), mColorTextures(),
		mKeyboard(nullptr),
		mClassName(className),
		mShaderName(shaderName), mModelName(modelName), mDiffuseMap(diffuseMap),
		mPosition(startPosition), mRotation(startRotation), mScale(startScale),
		mSkinnedModel(nullptr), mAnimationPlayer(nullptr),
		mRenderStateHelper(game), mSpriteBatch(nullptr), mSpriteFont(nullptr), mTextPosition(0.0f, 400.0f), mManualAdvanceMode(false),
		mDynCollider()
	{			
	}

	AnimatedGameObject::~AnimatedGameObject()
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

	void AnimatedGameObject::Initialize()
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
				//textureName << L"Content\\Models\\" << filename;
				textureName << mDiffuseMap.c_str();
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
		
		Scale(0,0,0);
		FirstRotation();
		Translate(mPosition);
		mDynCollider = new Colliders();
	}

	void AnimatedGameObject::Update(const GameTime& gameTime)
	{
		UpdateOptions();

		if (mManualAdvanceMode == false)
		{
			//mAnimationPlayer->Update(gameTime);
		}
	}

	void AnimatedGameObject::Draw(const GameTime& gameTime)
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
		/*
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
		*/
		if (mIsSelected && mIsEdited)
		{
			std::string precisionSwitch = (mPrecisionMode == true) ? "On" : "Off";

			helpLabel << "\nMode: " << mEditMode.c_str() <<
				" | Axis: " << mEditAxis.c_str() <<
				" | Edit factor: " << mEditFactor << " | Precision mode: " << precisionSwitch.c_str();
			helpLabel << "\nPosition: " << mPosition.x << ", " << mPosition.y << ", " << mPosition.z;
			helpLabel << "\nRotation: " << mRotation.x << ", " << mRotation.y << ", " << mRotation.z;
			helpLabel << "\nScale: " << mScale.x << ", " << mScale.y << ", " << mScale.z;
		}
		
		mSpriteFont->DrawString(mSpriteBatch, helpLabel.str().c_str(), mTextPosition);

		mSpriteBatch->End();
		mRenderStateHelper.RestoreAll();
	}

	// Transformation
	void AnimatedGameObject::Scale(float x, float y, float z)
	{
		XMFLOAT3 newScale = XMFLOAT3(
			mScale.x + x,
			mScale.y + y,
			mScale.z + z
		);
		mScale = newScale;
		XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(mScale.x, mScale.y, mScale.z));
	}

	void AnimatedGameObject::Scale(XMFLOAT3 scale)
	{
		XMFLOAT3 newScale = XMFLOAT3(
			mScale.x + scale.x,
			mScale.y + scale.y,
			mScale.z + scale.z
			);
		mScale = newScale;
		XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(mScale.x, mScale.y, mScale.z));
	}

	void AnimatedGameObject::Rotate(float x, float y, float z)
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

	void AnimatedGameObject::Rotate(XMFLOAT3 rotation)
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

	void AnimatedGameObject::FirstRotation()
	{
		XMMATRIX transformX = XMMatrixRotationX(XMConvertToRadians(mRotation.x));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformX);

		XMMATRIX transformY = XMMatrixRotationY(XMConvertToRadians(mRotation.y));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformY);

		XMMATRIX transformZ = XMMatrixRotationZ(XMConvertToRadians(mRotation.z));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformZ);
	}

	void AnimatedGameObject::Translate(float x, float y, float z)
	{
		XMFLOAT3 helperPos;
		helperPos.x = mPosition.x + x;
		helperPos.y = mPosition.y + y;
		helperPos.z = mPosition.z + z;
		XMMATRIX translate = XMMatrixTranslation(x, y, z);	

		if (mNode != nullptr && mDynCollider != nullptr)
		{
			if (NodeList::TryToMoveInNode(mDynCollider, XMLoadFloat3(&mPosition), MatrixHelper::MatrixZero(), translate.r[3], mNode))
			{
				mPosition = helperPos;
				XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * translate);
				return;
			}
		}
		else
		{
			mPosition = helperPos;
			XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * translate);
		}
	}

	void AnimatedGameObject::Translate(XMFLOAT3 translation)
	{
		XMMATRIX translate = XMMatrixTranslation(translation.x, translation.y, translation.z);

		if (mNode != nullptr && mDynCollider != nullptr)
		{
			if (NodeList::TryToMoveInNode(mDynCollider, XMLoadFloat3(&mPosition), MatrixHelper::MatrixZero(), translate.r[3], mNode))
			{
				XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * translate);
				return;
			}
		}
		else
		{
			XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * translate);
		}
	}

	void AnimatedGameObject::UpdateOptions()
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

			//if (mManualAdvanceMode && mKeyboard->WasKeyPressedThisFrame(DIK_SPACE))
			//{
				// Advance the current keyframe
				UINT currentKeyFrame = mAnimationPlayer->CurrentKeyframe();
				currentKeyFrame++;
				if (currentKeyFrame >= mAnimationPlayer->CurrentClip()->KeyframeCount())
				{
					currentKeyFrame = 0;
				}

				mAnimationPlayer->SetCurrentKeyFrame(currentKeyFrame);
			//}

			if(mIsSelected && mIsEdited)
				EditModel();
		}
	}

	// Creation Kit
	std::vector<std::string> AnimatedGameObject::Serialize()
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

	void AnimatedGameObject::EditModel()
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

	void AnimatedGameObject::ChangeEditFactor()
	{
		if (mKeyboard->WasKeyPressedThisFrame(DIK_NUMPADPLUS))
			mEditFactor += 0.05;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_NUMPADMINUS))
			mEditFactor -= 0.05;

		if (mEditFactor < 0)
			mEditFactor = 0;
	}

	void AnimatedGameObject::SetPosition()
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

	void AnimatedGameObject::SetRotation()
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

	void AnimatedGameObject::SetScale()
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

	void AnimatedGameObject::ChangeEditAxis()
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

	void AnimatedGameObject::ChangeEditMode()
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

	XMFLOAT3 AnimatedGameObject::getPosition()
	{
		return mPosition;
	}

	Colliders* AnimatedGameObject::getCollider()
	{
		return mDynCollider;
	}

	void  AnimatedGameObject::SetNode(CollisionNode* colNode)
	{
		inNode = colNode;
	}

	CollisionNode*  AnimatedGameObject::getNode()
	{
		return inNode;
	}

	void AnimatedGameObject::CheckTriggers()
	{

	}

	void AnimatedGameObject::BuildBoundingBox(XMFLOAT3 radius)
	{
			XMVECTOR helper;

			XMMATRIX transformX = XMMatrixRotationX(XMConvertToRadians(mRotation.x));
			XMMATRIX transformZ = XMMatrixRotationZ(XMConvertToRadians(mRotation.z));
			helper = XMLoadFloat3(&radius);

			helper = XMVector3Transform(helper, transformX);
			helper = XMVector3Transform(helper, transformZ);

			XMStoreFloat3(&radius, helper);
			mDynCollider->BuildBoundingBox(mPosition, radius);

	}
}