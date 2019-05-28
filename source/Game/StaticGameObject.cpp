#include "StaticGameObject.h"
#include "TextureMappingMaterial.h"
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

namespace Rendering
{
	RTTI_DEFINITIONS(StaticGameObject)

		StaticGameObject::StaticGameObject(Game& game, Camera& camera, const char *className, 
			const char *modelName, LPCWSTR shaderName, std::string diffuseMap,			
			XMFLOAT3 startPosition, XMFLOAT3 startRotation, XMFLOAT3 startScale, bool needCollision)
		: GameObject(game, camera, className,
			modelName, shaderName, diffuseMap,
			startPosition, startRotation, startScale),
		mMaterial(nullptr), needsCollision(needCollision)
	{
	}

	StaticGameObject::~StaticGameObject()
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
		DeleteObject(mMaterial);
		DeleteObject(mEffect);
	}

	void StaticGameObject::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		// Load the model
		mSkinnedModel = new Model(*mGame, mModelName, true);

		// Initialize the material
		mEffect = new Effect(*mGame);
		mEffect->LoadCompiledEffect(mShaderName);

		mMaterial = new TextureMappingMaterial();
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

		mSpriteBatch = new SpriteBatch(mGame->Direct3DDeviceContext());
		mSpriteFont = new SpriteFont(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");

		// Initial transform

		StaticGameObject::Scale(0, 0, 0);
		StaticGameObject::FirstRotation();
		StaticGameObject::Translate(mPosition);
		mCollider = new Colliders();
	}

	void StaticGameObject::Update(const GameTime& gameTime)
	{
		UpdateOptions();
	}

	void StaticGameObject::Draw(const GameTime& gameTime)
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

	void StaticGameObject::UpdateOptions()
	{
		if (mKeyboard != nullptr)
		{
			if (mIsSelected && mIsEdited)
				EditModel();
		}
	}

	void StaticGameObject::BuildBoundingBox(XMFLOAT3 radius)
	{
		if (needsCollision)
		{
			XMVECTOR helper;
			helper = XMVector3Transform(XMLoadFloat3(&radius), XMLoadFloat4x4(&mWorldMatrix));
			XMStoreFloat3(&radius, helper);
			mCollider = new Colliders();
			mCollider->BuildBoundingBox(mPosition, radius);

			if (inNode != nullptr)
				inNode->AddStaticCollider(mCollider);
		}
	}
}