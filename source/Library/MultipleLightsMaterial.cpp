#include "MultipleLightsMaterial.h"
#include "GameException.h"
#include "Mesh.h"
#include "Bone.h"
#include "BoneVertexWeights.h"

namespace Library
{
	RTTI_DEFINITIONS(MultipleLightsMaterial)

		MultipleLightsMaterial::MultipleLightsMaterial() :
		Material("Lights"),
		MATERIAL_VARIABLE_INITIALIZATION(DirectLights),
		MATERIAL_VARIABLE_INITIALIZATION(PointLights),
		MATERIAL_VARIABLE_INITIALIZATION(SpotLights),
		MATERIAL_VARIABLE_INITIALIZATION(WorldViewProjection),
		MATERIAL_VARIABLE_INITIALIZATION(World),
		MATERIAL_VARIABLE_INITIALIZATION(SpecularColor),
		MATERIAL_VARIABLE_INITIALIZATION(SpecularPower),
		MATERIAL_VARIABLE_INITIALIZATION(AmbientColor),
		MATERIAL_VARIABLE_INITIALIZATION(CameraPosition),
		MATERIAL_VARIABLE_INITIALIZATION(ColorTexture),
		MATERIAL_VARIABLE_INITIALIZATION(NormalTexture),
		MATERIAL_VARIABLE_INITIALIZATION(SpecularTexture)
	{
	}

	MATERIAL_VARIABLE_DEFINITION(MultipleLightsMaterial, DirectLights)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsMaterial, PointLights)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsMaterial, SpotLights)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsMaterial, WorldViewProjection)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsMaterial, World)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsMaterial, SpecularColor)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsMaterial, SpecularPower)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsMaterial, AmbientColor)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsMaterial, CameraPosition)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsMaterial, ColorTexture)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsMaterial, NormalTexture)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsMaterial, SpecularTexture)

		void MultipleLightsMaterial::Initialize(Effect* effect)
	{
		Material::Initialize(effect);

		MATERIAL_VARIABLE_RETRIEVE(DirectLights)
			MATERIAL_VARIABLE_RETRIEVE(PointLights)
			MATERIAL_VARIABLE_RETRIEVE(SpotLights)
			MATERIAL_VARIABLE_RETRIEVE(WorldViewProjection)
			MATERIAL_VARIABLE_RETRIEVE(World)
			MATERIAL_VARIABLE_RETRIEVE(SpecularColor)
			MATERIAL_VARIABLE_RETRIEVE(SpecularPower)
			MATERIAL_VARIABLE_RETRIEVE(AmbientColor)
			MATERIAL_VARIABLE_RETRIEVE(CameraPosition)
			MATERIAL_VARIABLE_RETRIEVE(ColorTexture)
			MATERIAL_VARIABLE_RETRIEVE(NormalTexture)
			MATERIAL_VARIABLE_RETRIEVE(SpecularTexture)

			D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		CreateInputLayout("Lights", "p0", inputElementDescriptions, ARRAYSIZE(inputElementDescriptions));
		CreateInputLayout("LightsNormal", "p0", inputElementDescriptions, ARRAYSIZE(inputElementDescriptions));
		CreateInputLayout("LightsSpecular", "p0", inputElementDescriptions, ARRAYSIZE(inputElementDescriptions));
		CreateInputLayout("LightsNormalSpecular", "p0", inputElementDescriptions, ARRAYSIZE(inputElementDescriptions));
	}

	void MultipleLightsMaterial::CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const
	{
		const std::vector<XMFLOAT3>& sourceVertices = mesh.Vertices();
		std::vector<XMFLOAT3>* textureCoordinates = mesh.TextureCoordinates().at(0);
		assert(textureCoordinates->size() == sourceVertices.size());
		const std::vector<XMFLOAT3>& normals = mesh.Normals();
		assert(normals.size() == sourceVertices.size());
		const std::vector<XMFLOAT3>& tangents = mesh.Tangents();
		assert(tangents.size() == sourceVertices.size());

		std::vector<VertexPositionTextureNormal> vertices;
		vertices.reserve(sourceVertices.size());
		for (UINT i = 0; i < sourceVertices.size(); i++)
		{
			XMFLOAT3 position = sourceVertices.at(i);
			XMFLOAT3 uv = textureCoordinates->at(i);
			XMFLOAT3 normal = normals.at(i);
			XMFLOAT3 tangent = tangents.at(i);

			vertices.push_back(VertexPositionTextureNormal(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y), normal, tangent));
		}

		CreateVertexBuffer(device, &vertices[0], vertices.size(), vertexBuffer);
	}

	void MultipleLightsMaterial::CreateVertexBuffer(ID3D11Device* device, VertexPositionTextureNormal* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const
	{
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.ByteWidth = VertexSize() * vertexCount;
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData;
		ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
		vertexSubResourceData.pSysMem = vertices;
		if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer)))
		{
			throw GameException("ID3D11Device::CreateBuffer() failed.");
		}
	}

	UINT MultipleLightsMaterial::VertexSize() const
	{
		return sizeof(VertexPositionTextureNormal);
	}
}

namespace Library
{
	RTTI_DEFINITIONS(MultipleLightsBonesMaterial)

		MultipleLightsBonesMaterial::MultipleLightsBonesMaterial() :
		Material("BonesLights"),
		MATERIAL_VARIABLE_INITIALIZATION(DirectLights),
		MATERIAL_VARIABLE_INITIALIZATION(PointLights),
		MATERIAL_VARIABLE_INITIALIZATION(SpotLights),
		MATERIAL_VARIABLE_INITIALIZATION(WorldViewProjection),
		MATERIAL_VARIABLE_INITIALIZATION(World),
		MATERIAL_VARIABLE_INITIALIZATION(SpecularColor),
		MATERIAL_VARIABLE_INITIALIZATION(SpecularPower),
		MATERIAL_VARIABLE_INITIALIZATION(AmbientColor),
		MATERIAL_VARIABLE_INITIALIZATION(CameraPosition),
		MATERIAL_VARIABLE_INITIALIZATION(ColorTexture),
		MATERIAL_VARIABLE_INITIALIZATION(NormalTexture),
		MATERIAL_VARIABLE_INITIALIZATION(SpecularTexture),
		MATERIAL_VARIABLE_INITIALIZATION(BoneTransforms)
	{
	}

	MATERIAL_VARIABLE_DEFINITION(MultipleLightsBonesMaterial, DirectLights)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsBonesMaterial, PointLights)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsBonesMaterial, SpotLights)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsBonesMaterial, WorldViewProjection)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsBonesMaterial, World)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsBonesMaterial, SpecularColor)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsBonesMaterial, SpecularPower)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsBonesMaterial, AmbientColor)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsBonesMaterial, CameraPosition)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsBonesMaterial, ColorTexture)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsBonesMaterial, NormalTexture)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsBonesMaterial, SpecularTexture)
		MATERIAL_VARIABLE_DEFINITION(MultipleLightsBonesMaterial, BoneTransforms)

		void MultipleLightsBonesMaterial::Initialize(Effect * effect)
	{
		Material::Initialize(effect);

		MATERIAL_VARIABLE_RETRIEVE(DirectLights)
			MATERIAL_VARIABLE_RETRIEVE(PointLights)
			MATERIAL_VARIABLE_RETRIEVE(SpotLights)
			MATERIAL_VARIABLE_RETRIEVE(WorldViewProjection)
			MATERIAL_VARIABLE_RETRIEVE(World)
			MATERIAL_VARIABLE_RETRIEVE(SpecularColor)
			MATERIAL_VARIABLE_RETRIEVE(SpecularPower)
			MATERIAL_VARIABLE_RETRIEVE(AmbientColor)
			MATERIAL_VARIABLE_RETRIEVE(CameraPosition)
			MATERIAL_VARIABLE_RETRIEVE(ColorTexture)
			MATERIAL_VARIABLE_RETRIEVE(NormalTexture)
			MATERIAL_VARIABLE_RETRIEVE(SpecularTexture)
			MATERIAL_VARIABLE_RETRIEVE(BoneTransforms)

			D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		CreateInputLayout("BonesLights", "p0", inputElementDescriptions, ARRAYSIZE(inputElementDescriptions));
		CreateInputLayout("BonesLightsNormal", "p0", inputElementDescriptions, ARRAYSIZE(inputElementDescriptions));
		CreateInputLayout("BonesLightsSpecular", "p0", inputElementDescriptions, ARRAYSIZE(inputElementDescriptions));
		CreateInputLayout("BonesLightsNormalSpecular", "p0", inputElementDescriptions, ARRAYSIZE(inputElementDescriptions));
	}

	void MultipleLightsBonesMaterial::CreateVertexBuffer(ID3D11Device * device, const Mesh & mesh, ID3D11Buffer ** vertexBuffer) const
	{
		const std::vector<XMFLOAT3>& sourceVertices = mesh.Vertices();
		std::vector<XMFLOAT3>* textureCoordinates = mesh.TextureCoordinates().at(0);
		assert(textureCoordinates->size() == sourceVertices.size());
		const std::vector<XMFLOAT3>& normals = mesh.Normals();
		assert(normals.size() == sourceVertices.size());
		const std::vector<XMFLOAT3>& tangents = mesh.Tangents();
		assert(tangents.size() == sourceVertices.size());
		const std::vector<BoneVertexWeights>& boneWeights = mesh.BoneWeights();
		assert(boneWeights.size() == sourceVertices.size());

		std::vector<VertexPositionTextureNormalBone> vertices;
		vertices.reserve(sourceVertices.size());
		for (UINT i = 0; i < sourceVertices.size(); i++)
		{
			XMFLOAT3 position = sourceVertices.at(i);
			XMFLOAT3 uv = textureCoordinates->at(i);
			XMFLOAT3 normal = normals.at(i);
			XMFLOAT3 tangent = tangents.at(i);
			BoneVertexWeights vertexWeights = boneWeights.at(i);
			auto vertexWeight = vertexWeights.Weights();

			while (vertexWeight.size() > 4)
			{
				auto min_it = std::min_element(vertexWeight.begin(), vertexWeight.end(),
					[](BoneVertexWeights::VertexWeight const& v1, BoneVertexWeights::VertexWeight const& v2)
				{
					return v1.Weight < v2.Weight;
				});

				if (min_it->Weight == 0)
					vertexWeight.erase(min_it);
				else
				{
					float erasedWeight = min_it->Weight * 100;
					vertexWeight.erase(min_it);

					for (BoneVertexWeights::VertexWeight weight : vertexWeight)
					{
						weight.Weight += weight.Weight * erasedWeight;
					}
				}
			}

			float weights[BoneVertexWeights::MaxBoneWeightsPerVertex];
			UINT indices[BoneVertexWeights::MaxBoneWeightsPerVertex];
			ZeroMemory(weights, sizeof(float) * ARRAYSIZE(weights));
			ZeroMemory(indices, sizeof(UINT) * ARRAYSIZE(indices));
			for (UINT i = 0; i < vertexWeight.size(); i++)
			{
				BoneVertexWeights::VertexWeight tmpVertexWeight = vertexWeight.at(i);
				weights[i] = tmpVertexWeight.Weight;
				indices[i] = tmpVertexWeight.BoneIndex;
			}

			vertices.push_back(VertexPositionTextureNormalBone(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y), normal, tangent, XMUINT4(indices), XMFLOAT4(weights)));
		}

		CreateVertexBuffer(device, &vertices[0], vertices.size(), vertexBuffer);
	}

	void MultipleLightsBonesMaterial::CreateVertexBuffer(ID3D11Device * device, VertexPositionTextureNormalBone * vertices, UINT vertexCount, ID3D11Buffer ** vertexBuffer) const
	{
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.ByteWidth = VertexSize() * vertexCount;
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData;
		ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
		vertexSubResourceData.pSysMem = vertices;
		if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer)))
		{
			throw GameException("ID3D11Device::CreateBuffer() failed.");
		}
	}

	UINT MultipleLightsBonesMaterial::VertexSize() const
	{
		return sizeof(VertexPositionTextureNormalBone);
	}
}