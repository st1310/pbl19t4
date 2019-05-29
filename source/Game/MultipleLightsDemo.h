#pragma once

#include "DrawableGameComponent.h"
#include "RenderStateHelper.h"

using namespace Library;

namespace Library
{
	class KeyboardComponent;
	class DirectionalLight;
	class PointLight;
	class SpotLight;
	class ProxyModel;
	class Effect;
	class MultipleLightsMaterial;
	class Model;
	class FullScreenRenderTarget;
	class FullScreenQuad;
	class ColorFilterMaterial;
}

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

namespace Rendering
{
	class MultipleLightsDemo :
		public DrawableGameComponent
	{
		RTTI_DECLARATIONS(MultipleLightsDemo, DrawableGameComponent)

		typedef struct _POINT_LIGHT
		{
			XMFLOAT3 Position;
			FLOAT LightRadius;
			XMFLOAT4 Color;
			bool Enabled;

			_POINT_LIGHT() {}

			_POINT_LIGHT(const XMFLOAT3& position, FLOAT radius, const XMFLOAT4& color, bool enabled) :
				Position(position), LightRadius(radius), Color(color), Enabled(enabled) {}
		} POINT_LIGHT;

	public:
		MultipleLightsDemo(Game& game, Camera& camera);
		~MultipleLightsDemo();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		MultipleLightsDemo();
		MultipleLightsDemo(const MultipleLightsDemo& rhs);
		MultipleLightsDemo& operator=(const MultipleLightsDemo& rhs);

		void UpdateColorFilterMaterial();
		void UpdateGenericColorFilter(const GameTime& gameTime);

		Effect* mEffect;
		MultipleLightsMaterial* mMaterial;

		KeyboardComponent* mKeyboard;

		std::vector<DirectionalLight*> mDirectLights;
		std::vector<PointLight*> mPointLights;
		std::vector<SpotLight*> mSpotLights;
		std::vector<ProxyModel*> mProxyModels;

		XMCOLOR mAmbientColor;
		XMCOLOR mSpecularColor;
		float mSpecularPower;

		XMFLOAT4X4 mWorldMatrix;

		std::vector<ID3D11Buffer*> mVertexBuffers;
		std::vector<ID3D11Buffer*> mIndexBuffers;
		std::vector<UINT> mIndexCounts;
		std::vector<ID3D11ShaderResourceView*> mColorTextures;
		std::vector<ID3D11ShaderResourceView*> mNormalTextures;

		ID3D11BlendState* mBlendState;
		RenderStateHelper mRenderStateHelper;

		Model* mModel;
	};
}
