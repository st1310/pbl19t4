﻿#include "AnimatedGameObject.h"
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
#include "AnimationSequence.h"
#include <WICTextureLoader.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <sstream>
#include <iomanip>
#include "Shlwapi.h"
#include "Colliders.h"
#include "NodeLIst.h"
#include "PointLight.h"

namespace Rendering
{
	RTTI_DEFINITIONS(AnimatedGameObject)

		AnimatedGameObject::AnimatedGameObject(Game& game, Camera& camera, const char *className,
			XMFLOAT3 startPosition, XMFLOAT3 startRotation, XMFLOAT3 startScale)
		: GameObject(game, camera, className, 
			startPosition, startRotation, startScale),
		mMaterial(nullptr), mAnimationPlayer(nullptr)
	{		
		mAnimations = std::map<std::string, int>();
		mAnimationSequence = new AnimationSequence("Idle");
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
		DeleteObject(mModel);
		DeleteObject(mAnimationPlayer);
		DeleteObject(mMaterial);
		DeleteObject(mEffect);
	}

	void AnimatedGameObject::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		// Load the model
		if (mModel == nullptr)
		{
			std::string modelName = "Content\\Models\\" + (std::string)mClassName + ".fbx";
			mModel = new Model(*mGame, modelName, true);
		}

		// Initialize the material
		mEffect = new Effect(*mGame);
		mEffect->LoadCompiledEffect(L"Content\\Effects\\SkinnedModel.cso");

		mMaterial = new SkinnedModelMaterial();
		mMaterial->Initialize(mEffect);

		// Create the vertex and index buffers
		mVertexBuffers.resize(mModel->Meshes().size());
		mIndexBuffers.resize(mModel->Meshes().size());
		mIndexCounts.resize(mModel->Meshes().size());
		mColorTextures.resize(mModel->Meshes().size());

		for (UINT i = 0; i < mModel->Meshes().size(); i++)
		{
			Mesh* mesh = mModel->Meshes().at(i);

			ID3D11Buffer* vertexBuffer = nullptr;
			mMaterial->CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, &vertexBuffer);
			mVertexBuffers[i] = vertexBuffer;

			ID3D11Buffer* indexBuffer = nullptr;
			mesh->CreateIndexBuffer(&indexBuffer);
			mIndexBuffers[i] = indexBuffer;

			mIndexCounts[i] = mesh->Indices().size();

			ID3D11ShaderResourceView* colorTexture = nullptr;
			ModelMaterial* material = mesh->GetMaterial();

			std::string modelName = "Content\\Textures\\" + (std::string)mClassName + "DiffuseMap.jpg";
			ChangeTexture(modelName);
		}

		mKeyboard = (KeyboardComponent*)mGame->Services().GetService(KeyboardComponent::TypeIdClass());
		assert(mKeyboard != nullptr);

		mAnimationPlayer = new AnimationPlayer(*mGame, *mModel, false);
		mAnimationPlayer->StartClip(*(mModel->Animations().at(0)));

		mSpriteBatch = new SpriteBatch(mGame->Direct3DDeviceContext());
		mSpriteFont = new SpriteFont(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");

		// Initial transform	
		Scale(0,0,0);
		FirstRotation();
		FirstTranslation(mPosition);
		mCollider = new Colliders();
		this->mCollider->Transform(XMLoadFloat4x4(&mWorldMatrix), XMLoadFloat3(&mPosition));
	}

	void AnimatedGameObject::Update(const GameTime& gameTime)
	{
		UpdateOptions();

		if (mState->IsInActiveState())
		{
			Move();
			mAnimationPlayer->Update(gameTime);
		}

		if (!mState->IsInActiveState() && mIsBusy && !getIsSelected())
		{
			mIsBusy = false;
			std::string modelName = "Content\\Textures\\" + (std::string)mClassName + "DiffuseMap.jpg";
			mAnimationSequence->EndLoop();
			ChangeTexture(modelName);
		}

		if (!mState->IsInActiveState() && mIsBusy && !getIsSelected())
		{
			mIsBusy = false;
			mAnimationSequence->EndLoop();
			ChangeTexture(mIsSelectedDiffuseMap);
		}

		if (mCurrentAnimation != mAnimationSequence->GetCurrentAnimation(mAnimationPlayer->CurrentTime()))
		{
			mCurrentAnimation = mAnimationSequence->GetCurrentAnimation(mAnimationPlayer->CurrentTime());
			ChangeAnimation(mCurrentAnimation);
		}

		if (mIsFolowable)
			GetCamera()->SetPosition(GetFollowPositionToCamera());

		mAnimationPlayer->Update(gameTime);
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
			// Camera
			if (mKeyboard->WasKeyPressedThisFrame(DIK_X) && mIsSelected)
				GetCamera()->SetPosition(GetFollowPositionToCamera());

			if (mKeyboard->WasKeyPressedThisFrame(DIK_Z) && mIsSelected)
				mIsFolowable = !mIsFolowable;

			if (mKeyboard->WasKeyPressedThisFrame(DIK_H) && mIsSelected)
				SetIsHidden(!mIsHidden);

			if(mIsSelected && mIsEdited)
				EditModel();
		}
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

	void AnimatedGameObject::BuildSphere(float radius)
	{
		mCollider->BuildSphere(mPosition, radius);
		if (inNode != nullptr)
			inNode->AddDynamicCollider(mCollider);
	}

	void AnimatedGameObject::ChangeAnimation(std::string animationName)
	{
		if (mAnimations.find(animationName) == std::end(mAnimations))
			return;

		int animationNumber = mAnimations.at(animationName);
		mAnimationPlayer->StartClip(*(mModel->Animations().at(animationNumber)));
	}

	void AnimatedGameObject::SetAnimations()
	{
	}

	void AnimatedGameObject::RunInit()
	{
		mAnimationSequence->InitLoopAnimationSequence("StartRunning", "Run", "StopRunning");
	}

	void AnimatedGameObject::PatrolInit()
	{
		mAnimationSequence->InitLoopAnimationSequence("Patrol", "Patrol", "Patrol");
	}

	void AnimatedGameObject::StartHidingInit()
	{
		mAnimationSequence->InitLoopAnimationSequence("StartHiding", "HideIdle", "HideIdle");
	}

	void AnimatedGameObject::EndHidingInit()
	{
		mAnimationSequence->InitLoopAnimationSequence("EndHiding", "Idle", "Idle");
	}

	XMFLOAT3 AnimatedGameObject::GetFollowPositionToCamera()
	{
		float zPosition = mPosition.z + 50;

		if (GetCamera()->Position().y > 50)
			zPosition += GetCamera()->Position().y * 0.2;

		if (GetCamera()->Position().y > 80)
			zPosition += GetCamera()->Position().y * 0.4;

		XMFLOAT3 newCameraPosition = XMFLOAT3(
			mPosition.x,
			GetCamera()->Position().y,
			zPosition
		);

		return newCameraPosition;
	}

	PointLight* AnimatedGameObject::GetPointLight()
	{
		return mPointLight;
	}

	void AnimatedGameObject::SetIsHidden(bool isHidden)
	{
		if (isHidden == mIsHidden || mClassName == "Policeman")
			return;

		mIsHidden = isHidden;

		if (mIsHidden)
		{
			StartHidingInit();
			GetPointLight()->SetRadius(0.0f);
		}			

		else
		{
			EndHidingInit();
			GetPointLight()->SetRadius(30.0f);
		}		
	}

	bool AnimatedGameObject::GetIsHidden()
	{
		return mIsHidden;
	}

	void AnimatedGameObject::StartMoving(std::vector<XMFLOAT2> positions, bool isLoopable)
	{
		GameObject::StartMoving(positions, isLoopable);
	}
}