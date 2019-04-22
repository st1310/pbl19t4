#include "TexturedModelMaterialDemo.h"
#include "Game.h"
#include "GameException.h"
#include "TextureMappingMaterial.h"
#include "Camera.h"
#include "MouseComponent.h"
#include "MatrixHelper.h"
#include "VectorHelper.h"
#include "../Library/Model.h"
#include "Mesh.h"
#include "Utility.h"
#include <DDSTextureLoader.h>

namespace Rendering
{
	RTTI_DEFINITIONS(TexturedModelMaterialDemo)

	TexturedModelMaterialDemo::TexturedModelMaterialDemo(Game& game, Camera& camera, const std::wstring& colorTextureFileName) :
		DrawableGameComponent(game, camera),
		mColorTextureFileName(colorTextureFileName), mEffect(nullptr), mMaterial(nullptr),
		mColorTextureShaderResourceView(nullptr), mVertexBuffer(nullptr), mIndexBuffer(nullptr), mIndexCount(0),
		mWorldMatrix(MatrixHelper::Identity)
	{
	}

	TexturedModelMaterialDemo::~TexturedModelMaterialDemo()
	{
		ReleaseObject(mColorTextureShaderResourceView);
		DeleteObject(mMaterial);
		DeleteObject(mEffect);
		ReleaseObject(mVertexBuffer);
		ReleaseObject(mIndexBuffer);
	}

	void TexturedModelMaterialDemo::Initialize()
	{
		mMouse = (MouseComponent*)mGame->Services().GetService(MouseComponent::TypeIdClass());

		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		std::unique_ptr<Model> model(new Model(*mGame, "Content\\Models\\Sphere.obj", true));
		//std::unique_ptr<Model> model(new Model(*mGame, "Content\\Models\\NVDemon.3DS", true));

		mEffect = new Effect(*mGame);
		mEffect->LoadCompiledEffect(L"Content\\Effects\\TextureMapping.cso");

		mMaterial = new TextureMappingMaterial();
		mMaterial->Initialize(mEffect);

		Mesh* mesh = model->Meshes().at(0);
		mMaterial->CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, &mVertexBuffer);
		mesh->CreateIndexBuffer(&mIndexBuffer);
		mIndexCount = mesh->Indices().size();

		HRESULT hr = DirectX::CreateDDSTextureFromFile(mGame->Direct3DDevice(), mColorTextureFileName.c_str(), nullptr, &mColorTextureShaderResourceView);
		if (FAILED(hr))
		{
			throw GameException("CreateDDSTextureFromFile() failed.", hr);
		}

		/*XMMATRIX transform = XMMatrixScaling(0.05f, 0.05f, 0.05f) * XMMatrixRotationX(XMConvertToRadians(-90.0f));
		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * transform);*/
	}

	void TexturedModelMaterialDemo::Update(const GameTime& gameTime)
	{
		XMFLOAT2 rotationAmount = Vector2Helper::Zero;
		if ((mMouse != nullptr) && (mMouse->IsButtonHeldDown(MouseButtonsRight)))
		{
			LPDIMOUSESTATE mouseState = mMouse->CurrentState();
			rotationAmount.x = -mouseState->lX * 100;
			rotationAmount.y = -mouseState->lY * 100;
		}

		float elapsedTime = (float)gameTime.ElapsedGameTime();
		XMVECTOR rotationVector = XMLoadFloat2(&rotationAmount) * XMConvertToRadians(1.0f) * elapsedTime;

		XMMATRIX pitchMatrix = XMMatrixRotationX(XMVectorGetY(rotationVector));
		XMMATRIX yawMatrix = XMMatrixRotationY(XMVectorGetX(rotationVector));
		XMMATRIX rotationMatrix = XMMatrixMultiply(pitchMatrix, yawMatrix);

		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mWorldMatrix) * rotationMatrix);
	}

	void TexturedModelMaterialDemo::Draw(const GameTime& gameTime)
	{
		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Pass* pass = mMaterial->CurrentTechnique()->Passes().at(0);
		ID3D11InputLayout* inputLayout = mMaterial->InputLayouts().at(pass);
		direct3DDeviceContext->IASetInputLayout(inputLayout);

		UINT stride = mMaterial->VertexSize();
		UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		XMMATRIX wvp = XMLoadFloat4x4(&mWorldMatrix) * mCamera->ViewMatrix() * mCamera->ProjectionMatrix();
		mMaterial->WorldViewProjection() << wvp;
		mMaterial->ColorTexture() << mColorTextureShaderResourceView;

		pass->Apply(0, direct3DDeviceContext);

		direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0);
	}
}
