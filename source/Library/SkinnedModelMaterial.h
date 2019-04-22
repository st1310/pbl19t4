#pragma once

#include "Common.h"
#include "Material.h"

namespace Library
{
	typedef struct _VertexSkinnedPositionTextureNormal
	{
		XMFLOAT4 Position;
		XMFLOAT2 TextureCoordinates;
		XMUINT4 BoneIndices;
		XMFLOAT4 BoneWeights;

		_VertexSkinnedPositionTextureNormal() { }

		_VertexSkinnedPositionTextureNormal(const XMFLOAT4& position, const XMFLOAT2& textureCoordinates, const XMUINT4& boneIndices, const XMFLOAT4& boneWeights)
			: Position(position), TextureCoordinates(textureCoordinates), BoneIndices(boneIndices), BoneWeights(boneWeights) { }
	} VertexSkinnedPositionTextureNormal;

	class SkinnedModelMaterial :
		public Material
	{
		RTTI_DECLARATIONS(SkinnedModelMaterial, Material)

		MATERIAL_VARIABLE_DECLARATION(WorldViewProjection)
		MATERIAL_VARIABLE_DECLARATION(BoneTransforms)
		MATERIAL_VARIABLE_DECLARATION(ColorTexture)

	public:
		SkinnedModelMaterial();

		virtual void Initialize(Effect* effect) override;
		virtual void CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const override;
		void CreateVertexBuffer(ID3D11Device* device, VertexSkinnedPositionTextureNormal* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const;
		virtual UINT VertexSize() const override;
	};
}