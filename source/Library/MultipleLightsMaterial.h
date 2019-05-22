#pragma once

#include "Common.h"
#include "Material.h"

namespace Library
{
	class MultipleLightsMaterial :
		public Material
	{
		RTTI_DECLARATIONS(MultipleLightsMaterial, Material)

		MATERIAL_VARIABLE_DECLARATION(PointLights)
		MATERIAL_VARIABLE_DECLARATION(WorldViewProjection)
		MATERIAL_VARIABLE_DECLARATION(World)
		MATERIAL_VARIABLE_DECLARATION(SpecularColor)
		MATERIAL_VARIABLE_DECLARATION(SpecularPower)
		MATERIAL_VARIABLE_DECLARATION(AmbientColor)
		MATERIAL_VARIABLE_DECLARATION(CameraPosition)
		MATERIAL_VARIABLE_DECLARATION(ColorTexture)

		typedef struct _VertexPositionTextureNormal
		{
			XMFLOAT4 Position;
			XMFLOAT2 TextureCoordinates;
			XMFLOAT3 Normal;

			_VertexPositionTextureNormal() { }

			_VertexPositionTextureNormal(const XMFLOAT4& position, const XMFLOAT2& textureCoordinates, const XMFLOAT3& normal)
				: Position(position), TextureCoordinates(textureCoordinates), Normal(normal) { }
		} VertexPositionTextureNormal;

	public:
		MultipleLightsMaterial();

		virtual void Initialize(Effect* effect) override;
		virtual void CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const override;
		void CreateVertexBuffer(ID3D11Device* device, VertexPositionTextureNormal* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const;
		virtual UINT VertexSize() const override;
	};
}
