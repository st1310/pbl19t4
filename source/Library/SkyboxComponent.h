#pragma once

#include "Common.h"
#include "DrawableGameComponent.h"

namespace Library
{
	class Effect;
	class SkyboxMaterial;

	class SkyboxComponent :
		public DrawableGameComponent
	{
		RTTI_DECLARATIONS(SkyboxComponent, DrawableGameComponent)

	public:
		SkyboxComponent(Game& game, Camera& camera, const std::wstring& cubeMapFileName, float scale);
		~SkyboxComponent();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;
		
	private:
		SkyboxComponent();
		SkyboxComponent(const SkyboxComponent& rhs);
		SkyboxComponent& operator=(const SkyboxComponent& rhs);

		std::wstring mCubeMapFileName;
		Effect* mEffect;
		SkyboxMaterial* mMaterial;
		ID3D11ShaderResourceView* mCubeMapShaderResourceView;
		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;

		XMFLOAT4X4 mWorldMatrix;
		XMFLOAT4X4 mScaleMatrix;
	};
}
