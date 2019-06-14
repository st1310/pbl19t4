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

		ID3D11ShaderResourceView* mOptionActionsBanner;
		ID3D11ShaderResourceView* mPortraitBanner;
		ID3D11ShaderResourceView* mUnitsBanner;
		ID3D11ShaderResourceView* mMultiSelectionPortrait;

		ID3D11ShaderResourceView* mOptionButtonYES;
		ID3D11ShaderResourceView* mOptionButtonMAYBE;
		ID3D11ShaderResourceView* mOptionButtonCLICKED;
		ID3D11ShaderResourceView* mOptionButtonNO;

		ID3D11ShaderResourceView* mPaintIconYES;
		ID3D11ShaderResourceView* mPaintIconMAYBE;
		ID3D11ShaderResourceView* mPaintIconCLICKED;
		ID3D11ShaderResourceView* mPaintIconNO;

		ID3D11ShaderResourceView* mCleanIconYES;
		ID3D11ShaderResourceView* mCleanIconMAYBE;
		ID3D11ShaderResourceView* mCleanIconCLICKED;
		ID3D11ShaderResourceView* mCleanIconNO;

		ID3D11ShaderResourceView* mPatrolIconYES;
		ID3D11ShaderResourceView* mPatrolIconMAYBE;
		ID3D11ShaderResourceView* mPatrolIconCLICKED;
		ID3D11ShaderResourceView* mPatrolIconNO;

		ID3D11ShaderResourceView* mStopIconYES;
		ID3D11ShaderResourceView* mStopIconMAYBE;
		ID3D11ShaderResourceView* mStopIconCLICKED;
		ID3D11ShaderResourceView* mStopIconNO;

		ID3D11ShaderResourceView* mHideIconYES;
		ID3D11ShaderResourceView* mHideIconMAYBE;
		ID3D11ShaderResourceView* mHideIconCLICKED;
		ID3D11ShaderResourceView* mHideIconNO;

		ID3D11ShaderResourceView* mNoiseIconYES;
		ID3D11ShaderResourceView* mNoiseIconMAYBE;
		ID3D11ShaderResourceView* mNoiseIconCLICKED;
		ID3D11ShaderResourceView* mNoiseIconNO;

		ID3D11ShaderResourceView* mTacticalMap;

		ID3D11ShaderResourceView* mTacticalButton1NO;
		ID3D11ShaderResourceView* mTacticalButton1MAYBE;
		ID3D11ShaderResourceView* mTacticalButton1YES;
		ID3D11ShaderResourceView* mTacticalButton1CLICKED;

		ID3D11ShaderResourceView* mTacticalButton2NO;
		ID3D11ShaderResourceView* mTacticalButton3NO;

		ID3D11ShaderResourceView* mStartButtonNO;
		ID3D11ShaderResourceView* mStartButtonMAYBE;
		ID3D11ShaderResourceView* mStartButtonCLICKED;

		XMFLOAT2 mMouseTextPosition;

		XMFLOAT2 mouse1Pos;
		XMFLOAT2 mouse2Pos;
		GameTime timeFromPressed;
		bool selectedOnce;

		Colliders* mCollC;
		Colliders* mCollTM;
		TexturedModelDemo* mTMDemo;
		GameManager* mGameManager;

		bool showUnitsGui = false;
		bool showUnitDetail = false;
		bool mapLevel = true;
		bool buttonWasHold = false;
		int whichUnitButtonIsClicking = -1;
		int whichTacticalMapButtonIsClicking = -1;
		int keybordButtonSelectUnit = -1;
		std::vector<int> indexSelectedGuiButtons;

		
	};
}