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
#include "AnimationSequence.h"
#include <WICTextureLoader.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <sstream>
#include <iomanip>
#include "Shlwapi.h"
#include "Colliders.h"
#include "MultipleLightsMaterial.h"
#include "SpotLight.h"
#include "ProxyModel.h"
#include "DirectionalLight.h"
#include "FullScreenRenderTarget.h"
#include "ColorFilterMaterial.h"
#include "FullScreenQuad.h"
#include <DDSTextureLoader.h>

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
		std::string modelName = "Content\\Models\\" + (std::string)mClassName + ".fbx";
		mModel = new Model(*mGame, modelName, true);

		// Initialize the material
		mEffect = new Effect(*mGame);
		mEffect->LoadCompiledEffect(L"Content\\Effects\\MultipleLights.cso");

		mMaterial = new MultipleLightsBonesMaterial();
		mMaterial->Initialize(mEffect);
		mMaterial->SetCurrentTechnique(mEffect->TechniquesByName().at("BonesLights"));

		// Create the vertex and index buffers
		mVertexBuffers.resize(mModel->Meshes().size());
		mIndexBuffers.resize(mModel->Meshes().size());
		mIndexCounts.resize(mModel->Meshes().size());
		mColorTextures.resize(mModel->Meshes().size());
		mNormalTextures.resize(mModel->Meshes().size());
		mSpecularTextures.resize(mModel->Meshes().size());

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
			ID3D11ShaderResourceView* normalTexture = nullptr;
			ID3D11ShaderResourceView* specularTexture = nullptr;
			ModelMaterial* material = mesh->GetMaterial();

			std::string modelName = "Content\\Textures\\" + (std::string)mClassName + "DiffuseMap.jpg";
			ChangeTexture(modelName);

			std::string textureName = "Content\\Textures\\checker.dds";
			std::wostringstream specular;
			specular << textureName.c_str();
			HRESULT hr = DirectX::CreateDDSTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), specular.str().c_str(), nullptr, &specularTexture);
			if (FAILED(hr))
			{
				throw GameException("CreateDDSTextureFromFile() failed.", hr);
			}

			mNormalTextures[i] = normalTexture;
			mSpecularTextures[i] = specularTexture;
		}

		XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(0.06f, 0.06f, 0.06f));

		mKeyboard = (KeyboardComponent*)mGame->Services().GetService(KeyboardComponent::TypeIdClass());
		assert(mKeyboard != nullptr);

		// Blend state
		D3D11_BLEND_DESC blendStateDesc{ 0 };
		blendStateDesc.RenderTarget[0].BlendEnable = true;
		blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO; // or _ONE
		blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		mGame->Direct3DDevice()->CreateBlendState(&blendStateDesc, &mBlendState);

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
		if (!mState->IsInActiveState() && mIsBusy == true)
		{
			mIsBusy = false;
			std::string modelName = "Content\\Textures\\" + (std::string)mClassName + "DiffuseMap.jpg";
			mAnimationSequence->EndLoop();
			ChangeTexture(modelName);
		}

		if (mCurrentAnimation != mAnimationSequence->GetCurrentAnimation(mAnimationPlayer->CurrentTime()))
		{
			mCurrentAnimation = mAnimationSequence->GetCurrentAnimation(mAnimationPlayer->CurrentTime());
			ChangeAnimation(mCurrentAnimation);
		}

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
		XMVECTOR ambientColor = XMLoadColor(&mAmbientColor);
		XMVECTOR specularColor = XMLoadColor(&mSpecularColor);

		UINT stride = mMaterial->VertexSize();
		UINT offset = 0;

		for (UINT i = 0; i < mVertexBuffers.size(); i++)
		{
			ID3D11Buffer* vertexBuffer = mVertexBuffers[i];
			ID3D11Buffer* indexBuffer = mIndexBuffers[i];
			UINT indexCount = mIndexCounts[i];
			ID3D11ShaderResourceView* colorTexture = mColorTextures[i];
			ID3D11ShaderResourceView* normalTexture = mNormalTextures[i];
			ID3D11ShaderResourceView* specularTexture = mSpecularTextures[i];

			direct3DDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
			direct3DDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

			mMaterial->WorldViewProjection() << wvp;
			mMaterial->World() << worldMatrix;
			mMaterial->SpecularColor() << specularColor;
			mMaterial->SpecularPower() << mSpecularPower;
			mMaterial->AmbientColor() << ambientColor;
			mMaterial->ColorTexture() << colorTexture;
			mMaterial->NormalTexture() << normalTexture;
			mMaterial->SpecularTexture() << specularTexture;
			mMaterial->CameraPosition() << mCamera->PositionVector();
			mMaterial->BoneTransforms() << mAnimationPlayer->BoneTransforms();

			for (size_t i = 0; i < mDirectLights.size(); i++)
			{
				ID3DX11EffectVariable* variable = mMaterial->DirectLights().GetVariable()->GetElement(i);
				variable->GetMemberByName("Direction")->AsVector()->SetFloatVector(reinterpret_cast<const float*>(&mDirectLights.at(i)->DirectionVector()));
				variable->GetMemberByName("Color")->AsVector()->SetFloatVector(reinterpret_cast<const float*>(&mDirectLights.at(i)->ColorVector()));
				variable->GetMemberByName("Enabled")->AsScalar()->SetBool(true);
			}

			for (size_t i = 0; i < mPointLights.size(); i++)
			{
				ID3DX11EffectVariable* variable = mMaterial->PointLights().GetVariable()->GetElement(i);
				variable->GetMemberByName("Position")->AsVector()->SetFloatVector(reinterpret_cast<const float*>(&mPointLights.at(i)->PositionVector()));
				variable->GetMemberByName("LightRadius")->AsScalar()->SetFloat(mPointLights.at(i)->Radius());
				variable->GetMemberByName("Color")->AsVector()->SetFloatVector(reinterpret_cast<const float*>(&mPointLights.at(i)->ColorVector()));
				variable->GetMemberByName("Enabled")->AsScalar()->SetBool(true);
			}


			for (size_t i = 0; i < mSpotLights.size(); i++)
			{
				ID3DX11EffectVariable* variable = mMaterial->SpotLights().GetVariable()->GetElement(i);
				variable->GetMemberByName("Position")->AsVector()->SetFloatVector(reinterpret_cast<const float*>(&mSpotLights.at(i)->PositionVector()));
				variable->GetMemberByName("Direction")->AsVector()->SetFloatVector(reinterpret_cast<const float*>(&mSpotLights.at(i)->DirectionVector()));
				variable->GetMemberByName("OuterAngle")->AsScalar()->SetFloat(mSpotLights.at(i)->OuterAngle());
				variable->GetMemberByName("InnerAngle")->AsScalar()->SetFloat(mSpotLights.at(i)->InnerAngle());
				variable->GetMemberByName("LightRadius")->AsScalar()->SetFloat(mSpotLights.at(i)->Radius());
				variable->GetMemberByName("Color")->AsVector()->SetFloatVector(reinterpret_cast<const float*>(&mSpotLights.at(i)->ColorVector()));
				variable->GetMemberByName("Enabled")->AsScalar()->SetBool(true);
			}

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
				mAnimationPlayer->StartClip(*(mModel->Animations().at(0)));
			}

			if (mKeyboard->WasKeyPressedThisFrame(DIK_I))
			{
				// Enable/disabled interpolation
				mAnimationPlayer->SetInterpolationEnabled(!mAnimationPlayer->InterpolationEnabled());
			}

			if (mKeyboard->WasKeyPressedThisFrame(DIK_F))
			{
				//mStartAnimation = true;
				/*
				mAnimationSequence->InitLoopAnimationSequence()
				mAnimationSequence.push_back("Paint");
				mAnimationSequence.push_back("Paint");
				mAnimationSequence.push_back("Paint");
				mAnimationSequence.push_back("Reload");
				mAnimationSequence.push_back("Paint");
				*/
			}

			if (mKeyboard->WasKeyPressedThisFrame(DIK_G))
			{
				//mStartAnimation = true;
				mAnimationSequence->InitLoopAnimationSequence("StartRunning", "Run", "StopRunning");
			}

			if (mKeyboard->WasKeyPressedThisFrame(DIK_B))
			{
				ChangeAnimation("Run");				
			}

			if (mKeyboard->WasKeyPressedThisFrame(DIK_V))
			{
				ChangeAnimation("Paint");
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
}