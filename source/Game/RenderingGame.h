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
	class MultipleLightsDemo;

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
		LPDIRECTINPUT8 mDirectInput;
		KeyboardComponent* mKeyboard;
		MouseComponent* mMouse;
		FirstPersonCamera* mCamera;
		FpsComponent* mFpsComponent;
		SkyboxComponent* mSkybox;

		// TODO: Add to Services and change components to use this one
		RenderStateHelper* mRenderStateHelper;

		SpriteBatch* mSpriteBatch;
		SpriteFont* mSpriteFont;
		XMFLOAT2 mMouseTextPosition;

		Colliders* mCollC;
		Colliders* mCollTM;
		MultipleLightsDemo* mMLDemo;
		GameManager* mGameManager;
	};
}