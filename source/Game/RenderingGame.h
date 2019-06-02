#pragma once

#include "Common.h"
#include "Game.h"
#include "Colliders.h"
#include "RenderStateHelper.h"

using namespace Library;

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

namespace Library
{
	class FpsComponent;
	class KeyboardComponent;
	class MouseComponent;
	class FirstPersonCamera;
	class GameCamera;
	class RenderStateHelper;
	class SkyboxComponent;
	class Effect;
	class FullScreenRenderTarget;
	class FullScreenQuad;
	class ColorFilterMaterial;
}

namespace Rendering
{
	class GameManager;
	class TexturedModelDemo;
	class TexturedModelMaterialDemo;
	class AnimationDemo;

	class RenderingGame : public Game
	{
	public:
		RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand);
		~RenderingGame();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	protected:
		virtual void Shutdown() override;

	private:


		static const XMVECTORF32 BackgroundColor;

		bool buttonClicked;
		bool unitActiveFlag;
		LPDIRECTINPUT8 mDirectInput;
		KeyboardComponent* mKeyboard;
		MouseComponent* mMouse;
		GameCamera* mCamera;
		//FirstPersonCamera* mCamera;
		FpsComponent* mFpsComponent;
		SkyboxComponent* mSkybox;

		// TODO: Add to Services and change components to use this one
		RenderStateHelper* mRenderStateHelper;

		SpriteBatch* mSpriteBatch;
		SpriteFont* mSpriteFont;
		ID3D11ShaderResourceView* mUnitGuiTexture;
		ID3D11ShaderResourceView* mUnitGuiTextureBlack;
		ID3D11ShaderResourceView* mSelectionRectangleTexture;
		XMFLOAT2 mMouseTextPosition;
	
		XMFLOAT2 mouse1Pos;
		XMFLOAT2 mouse2Pos;
		GameTime timeFromPressed;
		bool selectedOnce;

		Colliders* mCollC;
		Colliders* mCollTM;
		TexturedModelDemo* mTMDemo;
		GameManager* mGameManager;
	};
}