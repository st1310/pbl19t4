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
		: GameObject(game, camera, className, 
			modelName, shaderName, diffuseMap,
			startPosition, startRotation, startScale),
		mMaterial(nullptr), mAnimationPlayer(nullptr)
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

			ChangeTexture(mDiffuseMap.c_str());
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
		mCollider = new Colliders();
	}

	void AnimatedGameObject::Update(const GameTime& gameTime)
	{
		UpdateOptions();

		if (mState->IsInActiveState())
		{
			Move();
			mAnimationPlayer->Update(gameTime);
		}
		if (!mState->IsInActiveState() && mIsBusy == true)
		{
			mIsBusy = false;
			ChangeTexture(mDiffuseMap);
		}
		if (!mState->IsInActiveState() && mAnimationPlayer->CurrentKeyframe() != 0)
		{
			//float animationLenth = mSkinnedModel->Animations().at(0)->Duration();
			//mAnimationPlayer->CurrentKeyframe();
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
			helpLabel = GetCreationKitInfo();
		
		mSpriteFont->DrawString(mSpriteBatch, helpLabel.str().c_str(), mTextPosition);

		mSpriteBatch->End();
		mRenderStateHelper.RestoreAll();
	}

	void AnimatedGameObject::UpdateOptions()
	{
		if (mKeyboard != nullptr)
		{
			if (mKeyboard->WasKeyPressedThisFrame(DIK_P))
			{
				if (mAnimationPlayer->IsPlayingClip())
					mAnimationPlayer->PauseClip();

				else
					mAnimationPlayer->ResumeClip();
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
				UINT currentKeyFrame = mAnimationPlayer->CurrentKeyframe();
				currentKeyFrame++;

				if (currentKeyFrame >= mAnimationPlayer->CurrentClip()->KeyframeCount())
					currentKeyFrame = 0;

				mAnimationPlayer->SetCurrentKeyFrame(currentKeyFrame);
			}

			if(mIsSelected && mIsEdited)
				EditModel();
		}
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
			mCollider->BuildBoundingBox(mPosition, radius);
			if (inNode != nullptr)
				inNode->AddDynamicCollider(mCollider);
	}

}