//#pragma once
//#include "Common.h"
//#include "DrawableGameComponent.h"
//#include "FirstPersonCamera.h"
//#include "Scene.h"
//
//
//namespace Rendering
//{
//	class GUI : 
//		public DrawableGameComponent {
//
//		RTTI_DECLARATIONS(GameManager, DrawableGameComponent)
//
//	public:
//		GUI(Game& game, Camera& camera);
//		~GUI();
//
//		virtual void Initialize() override;
//		virtual void Update(const GameTime& gameTime) override;
//		virtual void Draw(const GameTime& gameTime) override;
//
//	private:
//		Game * game;
//		Camera* camera;
//
//		ID3D11ShaderResourceView * mUnitGuiTexture;
//		ID3D11ShaderResourceView* mUnitGuiTextureBlack;
//
//	};
//}