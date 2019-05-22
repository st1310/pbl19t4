#include "MultipleLightsDemo.h"
#include "Game.h"
#include "GameException.h"
#include "Camera.h"
#include "Utility.h"
#include "../Library/Model.h"
#include "Mesh.h"
#include "ModelMaterial.h"
#include "PointLight.h"
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
		mVertexBuffers(), mIndexBuffers(), mIndexCounts(), mColorTextures(),
		mKeyboard(nullptr), mAmbientColor(1.0f, 1.0f, 1.0f, 0.5f)/*mAmbientColor(reinterpret_cast<const float*>(&Colors::White))*/, mPointLights(),
		mSpecularColor(1.0f, 1.0f, 1.0f, 1.0f), mSpecularPower(25.0f), mModel(nullptr)
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

		for (PointLight* pointLight : mPointLights)
		{
			DeleteObject(pointLight);
		}

		DeleteObject(mModel);
		DeleteObject(mMaterial);
		DeleteObject(mEffect);
	}

	void MultipleLightsDemo::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		// Load the model
		mModel = new Model(*mGame, "Content\\Models\\Sphere.obj", true);

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

			/*if (material != nullptr && material->Textures().find(TextureTypeDifffuse) != material->Textures().cend())
			{
				std::vector<std::wstring>* diffuseTextures = material->Textures().at(TextureTypeDifffuse);
				std::wstring filename = PathFindFileName(diffuseTextures->at(0).c_str());

				std::wostringstream textureName;
				textureName << L"Content\\Textures\\" << L"EarthComposite" << L".jpg";
				HRESULT hr = DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), textureName.str().c_str(), nullptr, &colorTexture);
				if (FAILED(hr))
				{
					throw GameException("CreateWICTextureFromFile() failed.", hr);
				}
			}*/
			std::wostringstream textureName;
			textureName << L"Content\\Textures\\" << L"EarthComposite" << L".jpg";
			HRESULT hr = DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), textureName.str().c_str(), nullptr, &colorTexture);
			if (FAILED(hr))
			{
				throw GameException("CreateWICTextureFromFile() failed.", hr);
			}

			mColorTextures[i] = colorTexture;
		}

		//XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(0.05f, 0.05f, 0.05f));

		mKeyboard = (KeyboardComponent*)mGame->Services().GetService(KeyboardComponent::TypeIdClass());
		assert(mKeyboard != nullptr);
		
		mPointLights.push_back(new PointLight(*mGame));
		mPointLights.push_back(new PointLight(*mGame));

		mPointLights.at(0)->SetRadius(500.0f);
		mPointLights.at(0)->SetPosition(5.0f, 5.0f, 10.0f);
		mPointLights.at(0)->SetColor(Colors::Red);

		mPointLights.at(1)->SetRadius(500.0f);
		mPointLights.at(1)->SetPosition(-5.0f, -5.0f, 10.0f);
		mPointLights.at(1)->SetColor(Colors::Green);
	}

	void MultipleLightsDemo::Update(const GameTime& gameTime)
	{

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

			direct3DDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
			direct3DDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

			mMaterial->WorldViewProjection() << wvp;
			mMaterial->World() << worldMatrix;
			mMaterial->SpecularColor() << specularColor;
			mMaterial->SpecularPower() << mSpecularPower;
			mMaterial->AmbientColor() << ambientColor;
			mMaterial->ColorTexture() << colorTexture;
			mMaterial->CameraPosition() << mCamera->PositionVector();

			for (size_t i = 0; i < mPointLights.size(); i++)
			{
				ID3DX11EffectVariable* variable = mMaterial->PointLights().GetVariable()->GetElement(i);
				variable->GetMemberByName("Position")->AsVector()->SetFloatVector(reinterpret_cast<const float*>(&mPointLights.at(i)->PositionVector()));
				variable->GetMemberByName("LightRadius")->AsScalar()->SetFloat(mPointLights.at(i)->Radius());
				variable->GetMemberByName("Color")->AsVector()->SetFloatVector(reinterpret_cast<const float*>(&mPointLights.at(i)->ColorVector()));
				variable->GetMemberByName("Enabled")->AsScalar()->SetBool(true);
			}

			pass->Apply(0, direct3DDeviceContext);

			direct3DDeviceContext->DrawIndexed(indexCount, 0, 0);
		}
	}
}
