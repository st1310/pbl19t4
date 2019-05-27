#include "MultipleLightsDemo.h"
#include "Game.h"
#include "GameException.h"
#include "Camera.h"
#include "Utility.h"
#include "../Library/Model.h"
#include "Mesh.h"
#include "ModelMaterial.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "ProxyModel.h"
#include "KeyboardComponent.h"
#include "MultipleLightsMaterial.h"
#include <WICTextureLoader.h>
#include <DirectXColors.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "Shlwapi.h"

using namespace SimpleMath;

namespace Rendering
{
	RTTI_DEFINITIONS(MultipleLightsDemo)

	MultipleLightsDemo::MultipleLightsDemo(Game& game, Camera& camera) :
		DrawableGameComponent(game, camera),
		mMaterial(nullptr), mEffect(nullptr), mWorldMatrix(Matrix::Identity),
		mVertexBuffers(), mIndexBuffers(), mIndexCounts(), mColorTextures(), mNormalTextures(),
		mKeyboard(nullptr), mBlendState(), mRenderStateHelper(game),
		mDirectLights(), mPointLights(), mSpotLights(), mProxyModels(),
		mAmbientColor(1.0f, 1.0f, 1.0f, 0.2f)/*mAmbientColor(reinterpret_cast<const float*>(&Colors::White))*/,
		mSpecularColor(1.0f, 1.0f, 1.0f, 0.5f), mSpecularPower(25.0f), mModel(nullptr)
	{
	}

	MultipleLightsDemo::~MultipleLightsDemo()
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

		for (DirectionalLight* directLight : mDirectLights)
		{
			DeleteObject(directLight);
		}

		for (PointLight* pointLight : mPointLights)
		{
			DeleteObject(pointLight);
		}

		for (SpotLight* spotLight : mSpotLights)
		{
			DeleteObject(spotLight);
		}

		for (ProxyModel* proxyModel : mProxyModels)
		{
			DeleteObject(proxyModel);
		}

		DeleteObject(mModel);
		DeleteObject(mMaterial);
		DeleteObject(mEffect);

		ReleaseObject(mBlendState);
	}

	void MultipleLightsDemo::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		// Load the model
		mModel = new Model(*mGame, "Content\\Models\\Sponza\\Sponza.fbx", true);
		//mModel = new Model(*mGame, "Content\\Models\\Sphere.obj", true);

		// Initialize the material
		mEffect = new Effect(*mGame);
		mEffect->LoadCompiledEffect(L"Content\\Effects\\MultipleLights.cso");
		mMaterial = new MultipleLightsMaterial();
		mMaterial->Initialize(mEffect);

		// Create the vertex and index buffers
		mVertexBuffers.resize(mModel->Meshes().size());
		mIndexBuffers.resize(mModel->Meshes().size());
		mIndexCounts.resize(mModel->Meshes().size());
		mColorTextures.resize(mModel->Meshes().size());
		mNormalTextures.resize(mModel->Meshes().size());
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
			ModelMaterial* material = mesh->GetMaterial();

			if (material != nullptr && material->Textures().find(TextureTypeDifffuse) != material->Textures().cend())
			{
				std::vector<std::wstring>* diffuseTextures = material->Textures().at(TextureTypeDifffuse);
				std::wstring filename = PathFindFileName(diffuseTextures->at(0).c_str());

				std::wostringstream textureName;
				textureName << L"Content\\Models\\Sponza\\" << filename;
				HRESULT hr = DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), textureName.str().c_str(), nullptr, &colorTexture);
				if (FAILED(hr))
				{
					throw GameException("CreateWICTextureFromFile() failed.", hr);
				}
			}

			if (material != nullptr && material->Textures().find(TextureTypeHeightmap) != material->Textures().cend())
			{
				std::vector<std::wstring>* normalTextures = material->Textures().at(TextureTypeHeightmap);
				std::wstring filename = PathFindFileName(normalTextures->at(0).c_str());

				std::wostringstream textureName;
				textureName << L"Content\\Models\\Sponza\\" << filename;
				HRESULT hr = DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), textureName.str().c_str(), nullptr, &normalTexture);
				if (FAILED(hr))
				{
					throw GameException("CreateWICTextureFromFile() failed.", hr);
				}
			}
			//std::wostringstream textureName;
			////textureName << L"Content\\Textures\\" << L"EarthComposite" << L".jpg";
			//textureName << L"Content\\Models\\Sponza\\" << L"Lion_Albedo" << L".png";
			//HRESULT hr = DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), textureName.str().c_str(), nullptr, &colorTexture);
			//if (FAILED(hr))
			//{
			//	throw GameException("CreateWICTextureFromFile() failed.", hr);
			//}

			//textureName.str(L"");
			//textureName << L"Content\\Models\\Sponza\\" << L"Lion_Normal" << L".png";
			//hr = DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), textureName.str().c_str(), nullptr, &normalTexture);
			//if (FAILED(hr))
			//{
			//	throw GameException("CreateWICTextureFromFile() failed.", hr);
			//}

			mColorTextures[i] = colorTexture;
			mNormalTextures[i] = normalTexture;
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

		// Spot Lights
		/*mSpotLights.push_back(new SpotLight(*mGame));

		mSpotLights.at(0)->SetPosition(0.0f, 0.0f, 10.0f);
		mSpotLights.at(0)->SetRadius(50.0f);*/

		// Directional Lights
		mDirectLights.push_back(new DirectionalLight(*mGame));
		mDirectLights.back()->SetColor(Colors::AntiqueWhite - SimpleMath::Vector3(0.0f, 0.0f, 0.8f));
		mDirectLights.back()->ApplyRotation(XMMatrixRotationX(XMConvertToRadians(-90.0f)));
		mDirectLights.back()->ApplyRotation(XMMatrixRotationZ(XMConvertToRadians(45.0f)));
		mProxyModels.push_back(new ProxyModel(*mGame, *mCamera, "Content\\Models\\Proxy\\DirectionalLightProxy.obj"));
		mProxyModels.back()->SetPosition(XMFLOAT3(0.0f, 10.0f, 0.0f));
		mProxyModels.back()->ApplyRotation(XMMatrixRotationY(XMConvertToRadians(90.0f))); // Init rotation to allign with Vector3::Forward
		mProxyModels.back()->ApplyRotation(XMMatrixRotationX(XMConvertToRadians(-90.0f)));
		mProxyModels.back()->ApplyRotation(XMMatrixRotationZ(XMConvertToRadians(45.0f)));

		/*mDirectLights.push_back(new DirectionalLight(*mGame));

		mDirectLights.at(1)->ApplyRotation(XMMatrixRotationY(-90.0f));
		mDirectLights.at(1)->SetColor(Colors::Red);*/
		
		// Point Lights
		mPointLights.push_back(new PointLight(*mGame));
		mPointLights.at(0)->SetRadius(40.0f);
		mPointLights.at(0)->SetPosition(5.0f, 10.0f, -5.0f);
		mPointLights.at(0)->SetColor(Colors::Red - SimpleMath::Vector3(0.0f, 0.0f, 0.5f));
		mProxyModels.push_back(new ProxyModel(*mGame, *mCamera, "Content\\Models\\Proxy\\PointLightProxy.obj"));
		mProxyModels.back()->SetPosition(mPointLights.back()->Position());

		mPointLights.push_back(new PointLight(*mGame));
		mPointLights.at(1)->SetRadius(40.0f);
		mPointLights.at(1)->SetPosition(-5.0f, 10.0f, 5.0f);
		mPointLights.at(1)->SetColor(Colors::Green - SimpleMath::Vector3(0.0f, 0.0f, 0.5f));
		mProxyModels.push_back(new ProxyModel(*mGame, *mCamera, "Content\\Models\\Proxy\\PointLightProxy.obj"));
		mProxyModels.back()->SetPosition(mPointLights.back()->Position());

		for (ProxyModel* proxy : mProxyModels)
		{
			proxy->Initialize();
		}
	}

	void MultipleLightsDemo::Update(const GameTime& gameTime)
	{
		mPointLights.back()->SetPosition(mPointLights.back()->Position() + XMFLOAT3(sin(gameTime.TotalGameTime()) / 10, 0.0f, 0.0f));
		mProxyModels.back()->SetPosition(mPointLights.back()->Position());

		for (ProxyModel* proxy : mProxyModels)
		{
			proxy->Update(gameTime);
		}
	}

	void MultipleLightsDemo::Draw(const GameTime& gameTime)
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

			direct3DDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
			direct3DDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

			mMaterial->WorldViewProjection() << wvp;
			mMaterial->World() << worldMatrix;
			mMaterial->SpecularColor() << specularColor;
			mMaterial->SpecularPower() << mSpecularPower;
			mMaterial->AmbientColor() << ambientColor;
			mMaterial->ColorTexture() << colorTexture;
			mMaterial->NormalTexture() << normalTexture;
			mMaterial->CameraPosition() << mCamera->PositionVector();

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

			//mRenderStateHelper.SaveAll();

			//direct3DDeviceContext->OMSetBlendState(mBlendState, 0, 0xffffffff);

			pass->Apply(0, direct3DDeviceContext);

			direct3DDeviceContext->DrawIndexed(indexCount, 0, 0);

			//mRenderStateHelper.RestoreAll();
		}

		for (ProxyModel* proxy : mProxyModels)
		{
			proxy->Draw(gameTime);
		}
	}
}
