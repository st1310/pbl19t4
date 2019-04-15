#pragma once

#include "Common.h"
#include "Material.h"

namespace Library
{
	typedef struct _TextureMappingVertex
	{
		XMFLOAT4 Position;
		XMFLOAT2 TextureCoordinates;

		_TextureMappingVertex() { }

		_TextureMappingVertex(XMFLOAT4 position, XMFLOAT2 textureCoordinates)
			: Position(position), TextureCoordinates(textureCoordinates) { }
	} TextureMappingVertex;

	class TextureMappingMaterial :
		public Material
	{
		RTTI_DECLARATIONS(TextureMappingMaterial, Material)

		MATERIAL_VARIABLE_DECLARATION(WorldViewProjection)
		MATERIAL_VARIABLE_DECLARATION(ColorTexture)

	public:
		TextureMappingMaterial();

		virtual void Initialize(Effect* effect) override;
		virtual void CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const override;
		void CreateVertexBuffer(ID3D11Device* device, TextureMappingVertex* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const;
		virtual UINT VertexSize() const override;
	};
}