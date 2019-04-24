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

		GameObject::GameObject(Game& game, Camera& camera, const std::string modelName, const std::wstring shaderName, XMFLOAT4 position)
		: DrawableGameComponent(game, camera),
		mMaterial(nullptr), mEffect(nullptr), mWorldMatrix(MatrixHelper::Identity),
		mVertexBuffers(), mIndexBuffers(), mIndexCounts(), mColorTextures(),
		mKeyboard(nullptr),
		mShaderName(shaderName), mModelName(modelName),
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
		//mSkinnedModel = new Model(*mGame, "Content\\Models\\RunningSoldier.dae", true);		
		mSkinnedModel = new Model(*mGame, "Content\\Models\\Jednostka_green_baked.fbx", true);
		//mSkinnedModel = new Model(*mGame, mModelName, true);

		// Initialize the material
		mEffect = new Effect(*mGame);
		mEffect->LoadCompiledEffect(L"Content\\Effects\\SkinnedModel.cso");	
		//mEffect->LoadCompiledEffect(mShaderName.c_str());

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
				//textureName << L"Content\\Models\\" << filename.substr(0, filename.length() - 4) << L".png";
				//textureName << L"Content\\Models\\" << "Soldier.png";
				//textureName << L"Content\\Models\\" << "Monster.jpg";
				textureName << L"Content\\Models\\" << filename;
				HRESULT hr = DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), textureName.str().c_str(), nullptr, &colorTexture);
				if (FAILED(hr))
				{
					throw GameException("CreateWICTextureFromFile() failed.", hr);
				}
			}
			mColorTextures[i] = colorTexture;
		}

		GameObject::Scale(0.05f, 0.05f, 0.05f);

		mKeyboard = (KeyboardComponent*)mGame->Services().GetService(KeyboardComponent::TypeIdClass());
		assert(mKeyboard != nullptr);

		mAnimationPlayer = new AnimationPlayer(*mGame, *mSkinnedModel, false);
		mAnimationPlayer->StartClip(*(mSkinnedModel->Animations().at(0)));

		mSpriteBatch = new SpriteBatch(mGame->Direct3DDeviceContext());
		mSpriteFont = new SpriteFont(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");

		// Initial transform
		GameObject::Rotate(-90.0f, X_AXIS);
	}

	void GameObject::Update(const GameTime& gameTime)
	{
		UpdateOptions();

		if (mManualAdvanceMode == false)
		{
			mAnimationPlayer->Update(gameTime);
		}
		//GameObject::Rotate(-0.5f, Y_AXIS);
		//GameObject::Translate(0, 0, -0.005f);
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

		mSpriteFont->DrawString(mSpriteBatch, helpLabel.str().c_str(), mTextPosition);

		mSpriteBatch->End();
		mRenderStateHelper.RestoreAll();
	}

	// Transformation

	void GameObject::Scale(float x, float y, float z)
	{
		XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(x, y, z));
	}

	void GameObject::Rotate(float x, float y, float z)
	{
		XMMATRIX transformX = XMMatrixRotationX(XMConvertToRadians(x));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformX);

		XMMATRIX transformY = XMMatrixRotationY(XMConvertToRadians(y));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformY);

		XMMATRIX transformZ = XMMatrixRotationZ(XMConvertToRadians(z));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformZ);
	}

	void GameObject::Rotate(float angle, int axis)
	{
		switch (axis)
		{
		case X_AXIS:
		{
			XMMATRIX transformX = XMMatrixRotationX(XMConvertToRadians(angle));
			XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformX);
			return;
		}
		case Y_AXIS:
		{
			XMMATRIX transformY = XMMatrixRotationY(XMConvertToRadians(angle));
			XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformY);
			return;
		}
		case Z_AXIS:
		{
			XMMATRIX transformZ = XMMatrixRotationZ(XMConvertToRadians(angle));
			XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transformZ);
			return;
		}
		default:
		{
			return;
		}
		}
	}

	void GameObject::Translate(float x, float y, float z)
	{
		XMMATRIX translate = XMMatrixTranslation(x, y, z);	
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * translate);
	}

	void GameObject::Translate(float value, int axis)
	{
		switch (axis)
		{
		case X_AXIS:
		{
			XMMATRIX translate = XMMatrixTranslation(value, 0, 0);
			XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * translate);
			return;
		}
		case Y_AXIS:
		{
			XMMATRIX translate = XMMatrixTranslation(0, value, 0);
			XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * translate);
			return;
		}
		case Z_AXIS:
		{
			XMMATRIX translate = XMMatrixTranslation(0, 0, value);
			XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * translate);
			return;
		}
		default:
		{
			return;
		}
		}
	}

	void GameObject::UpdateOptions()
	{
		if (mKeyboard != nullptr)
		{
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
		}
	}


}