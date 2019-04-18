#include "SkinnedModelMaterial.h"
#include "GameException.h"
#include "Mesh.h"
#include "Bone.h"
#include "BoneVertexWeights.h"
#include <algorithm>
#include <iterator>

namespace Library
{
	RTTI_DEFINITIONS(SkinnedModelMaterial)

	SkinnedModelMaterial::SkinnedModelMaterial() :
		Material("main11"),
		MATERIAL_VARIABLE_INITIALIZATION(WorldViewProjection),
		MATERIAL_VARIABLE_INITIALIZATION(BoneTransforms),
		MATERIAL_VARIABLE_INITIALIZATION(ColorTexture)
	{
	}

	MATERIAL_VARIABLE_DEFINITION(SkinnedModelMaterial, WorldViewProjection)
	MATERIAL_VARIABLE_DEFINITION(SkinnedModelMaterial, BoneTransforms)
	MATERIAL_VARIABLE_DEFINITION(SkinnedModelMaterial, ColorTexture)

	void SkinnedModelMaterial::Initialize(Effect* effect)
	{
		Material::Initialize(effect);

		MATERIAL_VARIABLE_RETRIEVE(WorldViewProjection)
		MATERIAL_VARIABLE_RETRIEVE(BoneTransforms)
		MATERIAL_VARIABLE_RETRIEVE(ColorTexture)

		D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		CreateInputLayout("main11", "p0", inputElementDescriptions, ARRAYSIZE(inputElementDescriptions));
	}

	void SkinnedModelMaterial::CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const
	{
		const std::vector<XMFLOAT3>& sourceVertices = mesh.Vertices();
		std::vector<XMFLOAT3>* textureCoordinates = mesh.TextureCoordinates().at(0);
		assert(textureCoordinates->size() == sourceVertices.size());
		const std::vector<BoneVertexWeights>& boneWeights = mesh.BoneWeights();
		assert(boneWeights.size() == sourceVertices.size());

		std::vector<VertexSkinnedPositionTextureNormal> vertices;
		vertices.reserve(sourceVertices.size());
		for (UINT i = 0; i < sourceVertices.size(); i++)
		{
			XMFLOAT3 position = sourceVertices.at(i);
			XMFLOAT3 uv = textureCoordinates->at(i);
			BoneVertexWeights vertexWeights = boneWeights.at(i);
			auto vertexWeight = vertexWeights.Weights();

			////
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
			////

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

			vertices.push_back(VertexSkinnedPositionTextureNormal(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y), XMUINT4(indices), XMFLOAT4(weights)));
		}

		CreateVertexBuffer(device, &vertices[0], vertices.size(), vertexBuffer);
	}

	void SkinnedModelMaterial::CreateVertexBuffer(ID3D11Device* device, VertexSkinnedPositionTextureNormal* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const
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

	UINT SkinnedModelMaterial::VertexSize() const
	{
		return sizeof(VertexSkinnedPositionTextureNormal);
	}
}