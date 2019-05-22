#pragma once

#include "DrawableGameComponent.h"
#include "RenderStateHelper.h"

using namespace Library;

namespace Library
{
	class KeyboardComponent;
	class PointLight;
	class ProxyModel;
	class Effect;
	class MultipleLightsMaterial;
	class Model;
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

		Effect* mEffect;
		MultipleLightsMaterial* mMaterial;

		KeyboardComponent* mKeyboard;

		std::vector<PointLight*> mPointLights;
		XMCOLOR mAmbientColor;
		XMCOLOR mSpecularColor;
		float mSpecularPower;

		XMFLOAT4X4 mWorldMatrix;

		std::vector<ID3D11Buffer*> mVertexBuffers;
		std::vector<ID3D11Buffer*> mIndexBuffers;
		std::vector<UINT> mIndexCounts;
		std::vector<ID3D11ShaderResourceView*> mColorTextures;

		Model* mModel;
	};
}
