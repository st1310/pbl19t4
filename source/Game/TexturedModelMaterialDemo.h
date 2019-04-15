#pragma once

#include "Common.h"
#include "DrawableGameComponent.h"

using namespace Library;

namespace Library
{
	class Effect;
	class TextureMappingMaterial;
	class MouseComponent;
}

namespace Rendering
{
	class TexturedModelMaterialDemo :
		public DrawableGameComponent
	{
		RTTI_DECLARATIONS(TexturedModelMaterialDemo, DrawableGameComponent)

	public:
		TexturedModelMaterialDemo(Game& game, Camera& camera, const std::wstring& colorTextureFileName);
		~TexturedModelMaterialDemo();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		TexturedModelMaterialDemo();
		TexturedModelMaterialDemo(const TexturedModelMaterialDemo& rhs);
		TexturedModelMaterialDemo& operator=(const TexturedModelMaterialDemo& rhs);

		std::wstring mColorTextureFileName;
		Effect* mEffect;
		TextureMappingMaterial* mMaterial;
		ID3D11ShaderResourceView* mColorTextureShaderResourceView;
		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;

		MouseComponent* mMouse;

		XMFLOAT4X4 mWorldMatrix;
	};
}
