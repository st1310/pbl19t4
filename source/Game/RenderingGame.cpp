#include "RenderingGame.h"
#include <sstream>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "GameException.h"
#include "KeyboardComponent.h"
#include "MouseComponent.h"
#include "FpsComponent.h"
#include "Camera.h"
#include "FirstPersonCamera.h"
#include "GameCamera.h"
#include "SkyboxComponent.h"
#include "ColorHelper.h"
#include "RenderStateHelper.h"
#include "TexturedModelMaterialDemo.h"
#include "GameManager.h"
#include "AnimatedGameObject.h"
#include "Utility.h"
#include "NodeList.h"
#include <WICTextureLoader.h>
#include "MultipleLightsMaterial.h"
#include "FullScreenRenderTarget.h"
#include "FullScreenQuad.h"
#include "ColorFilterMaterial.h"
#include <CommonStates.h>

namespace Rendering
{
	const XMVECTORF32 RenderingGame::BackgroundColor = ColorHelper::CornflowerBlue;
	//const XMVECTORF32 RenderingGame::BackgroundColor = Colors::CornflowerBlue;
	
	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring & windowClass, const std::wstring & windowTitle, int showCommand)
		: Game(instance, windowClass, windowTitle, showCommand),
		mFpsComponent(nullptr), mSkybox(nullptr),
		mRenderStateHelper(nullptr),
		mDirectInput(nullptr), mKeyboard(nullptr), mMouse(nullptr), mCamera(nullptr),
		mSpriteBatch(nullptr), mSpriteFont(nullptr), mMouseTextPosition(0.0f, 20.0f),
		mGameManager(nullptr), buttonClicked(false), unitActiveFlag(false), mSelectionRectangleTexture(nullptr), mOptionActionsBanner(nullptr)
	{
		mDepthStencilBufferEnabled = true;
		mMultiSamplingEnabled = true;
		indexSelectedGuiButtons.clear();
	}

	RenderingGame::~RenderingGame()
	{
	}

	void RenderingGame::Initialize()
	{
		if (FAILED(DirectInput8Create(mInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&mDirectInput, nullptr)))
			throw GameException("DirectInput8Create() failed");

		mKeyboard = new KeyboardComponent(*this, mDirectInput);
		mComponents.push_back(mKeyboard);
		mServices.AddService(KeyboardComponent::TypeIdClass(), mKeyboard);

		mMouse = new MouseComponent(*this, mDirectInput);
		mComponents.push_back(mMouse);
		mServices.AddService(MouseComponent::TypeIdClass(), mMouse);

		mCollC = new Colliders();
		mCamera = new GameCamera(*this);
		CollisionNode* newNode = new CollisionNode({-5.f, -20.f, 0.f}, { 5.f, 20.f, 50.f });

		mComponents.push_back(mCamera);
		mServices.AddService(GameCamera::TypeIdClass(), mCamera);

		mSkybox = new SkyboxComponent(*this, *mCamera, L"Content\\Textures\\Maskonaive2_1024.dds", 100.0f);
		mComponents.push_back(mSkybox);
		mServices.AddService(SkyboxComponent::TypeIdClass(), mSkybox);

		mGameManager = new GameManager(*this, *mCamera);
		mComponents.push_back(mGameManager);
		mGameManager->SetKeyboard(mKeyboard);

		mFpsComponent = new FpsComponent(*this); // Components using SpriteBach should perform Draw last
		mComponents.push_back(mFpsComponent);

		mRenderStateHelper = new RenderStateHelper(*this);

		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		mSpriteBatch = new SpriteBatch(mDirect3DDeviceContext);
		mSpriteFont = new SpriteFont(mDirect3DDevice, L"Content\\Fonts\\Arial_14_Regular.spritefont");




		std::wostringstream mGreenSoldierNOtexture;
		mGreenSoldierNOtexture << L"content\\Textures\\GreenSoldierNO.png";

		std::wostringstream mGreenSoldierYEStexture;
		mGreenSoldierYEStexture << L"content\\Textures\\GreenSoldierYES.png";

		std::wostringstream mGreenSoldierMAYBEtexture;
		mGreenSoldierMAYBEtexture << L"content\\Textures\\GreenSoldierMAYBE.png";

		std::wostringstream mGreenSoldierCLICKEDtexture;
		mGreenSoldierCLICKEDtexture << L"content\\Textures\\GreenSoldierCLICKED.png";

		std::wostringstream mPaintSoldierNOtexture;
		mPaintSoldierNOtexture << L"content\\Textures\\PaintSoldierNO.png";
		
		std::wostringstream mPaintSoldierYEStexture;
		mPaintSoldierYEStexture << L"content\\Textures\\PaintSoldierYES.png";

		std::wostringstream mPaintSoldierMAYBEtexture;
		mPaintSoldierMAYBEtexture << L"content\\Textures\\PaintSoldierMAYBE.png";

		std::wostringstream mPaintSoldierCLICKEDtexture;
		mPaintSoldierCLICKEDtexture << L"content\\Textures\\PaintSoldierCLICKED.png";

		std::wostringstream textureForRactangle;
		textureForRactangle << L"content\\Textures\\whiteRect75.png";

		std::wostringstream textureOptionActionsBanner;
		textureOptionActionsBanner << L"content\\Textures\\optionsFrameSoldier.png";

		std::wostringstream textureOptionActionsBanner1;
		textureOptionActionsBanner1 << L"content\\Textures\\optionsFramePaint.png";

		std::wostringstream texturePortraitBanner;
		texturePortraitBanner << L"content\\Textures\\characterPortraitFrame.png";

		std::wostringstream textureUnitsBanner;
		textureUnitsBanner << L"content\\Textures\\UnitsBanner.png";

		std::wostringstream mPaintIconYEStexture;
		mPaintIconYEStexture << L"content\\Textures\\sprayYES.png";

		std::wostringstream mPaintIconMAYBEtexture;
		mPaintIconMAYBEtexture << L"content\\Textures\\sprayMAYBE.png";

		std::wostringstream mPaintIconCLICKEDtexture;
		mPaintIconCLICKEDtexture << L"content\\Textures\\sprayCLICKED.png";

		std::wostringstream mPaintIconNOtexture;
		mPaintIconNOtexture << L"content\\Textures\\sprayNO.png";

		std::wostringstream mCleanIconYEStexture;
		mCleanIconYEStexture << L"content\\Textures\\cleanYES.png";

		std::wostringstream mCleanIconMAYBEtexture;
		mCleanIconMAYBEtexture << L"content\\Textures\\cleanMAYBE.png";

		std::wostringstream mCleanIconCLICKEDtexture;
		mCleanIconCLICKEDtexture << L"content\\Textures\\cleanCLICKED.png";

		std::wostringstream mCleanIconNOtexture;
		mCleanIconNOtexture << L"content\\Textures\\cleanNO.png";

		std::wostringstream mPatrolIconMAYBEtexture;
		mPatrolIconMAYBEtexture << L"content\\Textures\\patrolMAYBE.png";

		std::wostringstream mPatrolIconCLICKEDtexture;
		mPatrolIconCLICKEDtexture << L"content\\Textures\\patrolCLICKED.png";

		std::wostringstream mPatrolIconYEStexture;
		mPatrolIconYEStexture << L"content\\Textures\\patrolYES.png";

		std::wostringstream mPatrolIconNOtexture;
		mPatrolIconNOtexture << L"content\\Textures\\patrolNO.png";

		std::wostringstream mStopIconMAYBEtexture;
		mStopIconMAYBEtexture << L"content\\Textures\\stopMAYBE.png";

		std::wostringstream mStopIconCLICKEDtexture;
		mStopIconCLICKEDtexture << L"content\\Textures\\stopCLICKED.png";

		std::wostringstream mStopIconNOtexture;
		mStopIconNOtexture << L"content\\Textures\\stopNO.png";

		std::wostringstream mStopIconYEStexture;
		mStopIconYEStexture << L"content\\Textures\\stopYES.png";

		std::wostringstream mHideIconNOtexture;
		mHideIconNOtexture << L"content\\Textures\\hideNO.png";

		std::wostringstream mHideIconCLICKEDtexture;
		mHideIconCLICKEDtexture << L"content\\Textures\\hideCLICKED.png";

		std::wostringstream mHideIconMAYBEtexture;
		mHideIconMAYBEtexture << L"content\\Textures\\hideMAYBE.png";

		std::wostringstream mHideIconYEStexture;
		mHideIconYEStexture << L"content\\Textures\\hideYES.png";

		std::wostringstream mNoiseIconYEStexture;
		mNoiseIconYEStexture << L"content\\Textures\\noiseYES.png";

		std::wostringstream mNoiseIconCLICKEDtexture;
		mNoiseIconCLICKEDtexture << L"content\\Textures\\noiseCLICKED.png";

		std::wostringstream mNoiseIconMAYBEtexture;
		mNoiseIconMAYBEtexture << L"content\\Textures\\noiseMAYBE.png";

		std::wostringstream mNoiseIconNOtexture;
		mNoiseIconNOtexture << L"content\\Textures\\noiseNO.png";

		std::wostringstream mMultiSelectionPortraittexture;
		mMultiSelectionPortraittexture << L"content\\Textures\\multiSelectionPortrait.png";

		std::wostringstream mOptionButtonYEStexture;
		mOptionButtonYEStexture << L"content\\Textures\\menuYES.png";

		std::wostringstream mOptionButtonMAYBEtexture;
		mOptionButtonMAYBEtexture << L"content\\Textures\\menuMAYBE.png";

		std::wostringstream mOptionButtonCLICKEDtexture;
		mOptionButtonCLICKEDtexture << L"content\\Textures\\menuCLICKED.png";

		std::wostringstream mOptionButtonNOtexture;
		mOptionButtonNOtexture << L"content\\Textures\\menuNO.png";
		
		std::wostringstream mTacticalMapTexture;
		mTacticalMapTexture << L"content\\Textures\\Map2.png";

		std::wostringstream mTacticalButton1NOtexture;
		mTacticalButton1NOtexture << L"content\\Textures\\TacticalButton1NO.png";

		std::wostringstream mTacticalButton1YEStexture;
		mTacticalButton1YEStexture << L"content\\Textures\\TacticalButton1YES.png";

		std::wostringstream mTacticalButton1MAYBEtexture;
		mTacticalButton1MAYBEtexture << L"content\\Textures\\TacticalButton1MAYBE.png";

		std::wostringstream mTacticalButton1CLICKEDtexture;
		mTacticalButton1CLICKEDtexture << L"content\\Textures\\TacticalButton1CLICKED.png";

		std::wostringstream mTacticalButton2NOtexture;
		mTacticalButton2NOtexture << L"content\\Textures\\TacticalButton2NO.png";

		std::wostringstream mTacticalButton2YEStexture;
		mTacticalButton2YEStexture << L"content\\Textures\\TacticalButton2YES.png";

		std::wostringstream mTacticalButton2MAYBEtexture;
		mTacticalButton2MAYBEtexture << L"content\\Textures\\TacticalButton2MAYBE.png";

		std::wostringstream mTacticalButton2CLICKEDtexture;
		mTacticalButton2CLICKEDtexture << L"content\\Textures\\TacticalButton2CLICKED.png";

		std::wostringstream mTacticalButton3NOtexture;
		mTacticalButton3NOtexture << L"content\\Textures\\TacticalButton3NO.png";

		std::wostringstream mTacticalButton3YEStexture;
		mTacticalButton3YEStexture << L"content\\Textures\\TacticalButton3YES.png";

		std::wostringstream mTacticalButton3MAYBEtexture;
		mTacticalButton3MAYBEtexture << L"content\\Textures\\TacticalButton3MAYBE.png";

		std::wostringstream mTacticalButton3CLICKEDtexture;
		mTacticalButton3CLICKEDtexture << L"content\\Textures\\TacticalButton3CLICKED.png";

		std::wostringstream mStartButtonNOtexture;
		mStartButtonNOtexture << L"content\\Textures\\StartButtonNO.png";

		std::wostringstream mStartButtonMAYBEtexture;
		mStartButtonMAYBEtexture << L"content\\Textures\\StartButtonMAYBE.png";

		std::wostringstream mStartButtonCLICKEDtexture;
		mStartButtonCLICKEDtexture << L"content\\Textures\\StartButtonCLICKED.png";


		std::wostringstream mshowPainter;
		mshowPainter << L"content\\Textures\\showPainter.png";

		std::wostringstream mshowPainter1;
		mshowPainter1 << L"content\\Textures\\showPainter1.png";

		std::wostringstream mshowPainter2;
		mshowPainter2 << L"content\\Textures\\showPainter2.png";

		std::wostringstream mshowPainter3;
		mshowPainter3 << L"content\\Textures\\showPainter3.png";

		std::wostringstream mbuttonXNOtex;
		mbuttonXNOtex << L"content\\Textures\\buttonXNO.png";

		std::wostringstream mbuttonXMAYBEtex;
		mbuttonXMAYBEtex << L"content\\Textures\\buttonXMAYBE.png";

		std::wostringstream mbuttonXCLICKEDtex;
		mbuttonXCLICKEDtex << L"content\\Textures\\buttonXCLICKED.png";

		std::wostringstream mcloud1texture;
		mcloud1texture << L"content\\Textures\\cloud1.png";

		std::wostringstream mcloud2texture;
		mcloud2texture << L"content\\Textures\\cloud2.png";

		std::wostringstream mcloud3texture;
		mcloud3texture << L"content\\Textures\\cloud3.png";

		std::wostringstream mcloud4texture;
		mcloud4texture << L"content\\Textures\\cloud4.png";

		std::wostringstream mcloud5texture;
		mcloud5texture << L"content\\Textures\\cloud5.png";

		std::wostringstream mcloud6texture;
		mcloud6texture << L"content\\Textures\\cloud6.png";

		std::wostringstream mcloud7texture;
		mcloud7texture << L"content\\Textures\\cloud7.png";
		
		std::wostringstream mPatrolModeBannerSoldiertexture;
		mPatrolModeBannerSoldiertexture << L"content\\Textures\\patrolModeSoldier.png";

		std::wostringstream mPatrolModeBannerPainttexture;
		mPatrolModeBannerPainttexture << L"content\\Textures\\patrolModePaint.png";

		HRESULT hr = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mGreenSoldierNOtexture.str().c_str(), nullptr, &mGreenSoldierNO);
		HRESULT hr1 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mGreenSoldierYEStexture.str().c_str(), nullptr, &mGreenSoldierYES);
		HRESULT hr01 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mGreenSoldierMAYBEtexture.str().c_str(), nullptr, &mGreenSoldierMAYBE);
		HRESULT hr00 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mGreenSoldierCLICKEDtexture.str().c_str(), nullptr, &mGreenSoldierCLICKED);
		HRESULT hr2 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), textureForRactangle.str().c_str(), nullptr, &mSelectionRectangleTexture);
		HRESULT hr3 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), textureOptionActionsBanner.str().c_str(), nullptr, &mOptionActionsBanner);
		HRESULT hr4 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), texturePortraitBanner.str().c_str(), nullptr, &mPortraitBanner);
		HRESULT hr5 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), textureUnitsBanner.str().c_str(), nullptr, &mUnitsBanner);
		HRESULT hr6 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mPaintIconYEStexture.str().c_str(), nullptr, &mPaintIconYES);
		HRESULT hr7 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mPaintIconNOtexture.str().c_str(), nullptr, &mPaintIconNO);
		HRESULT hr8 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mCleanIconYEStexture.str().c_str(), nullptr, &mCleanIconYES);
		HRESULT hr9 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mCleanIconNOtexture.str().c_str(), nullptr, &mCleanIconNO);
		HRESULT hr10 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mPatrolIconYEStexture.str().c_str(), nullptr, &mPatrolIconYES);
		HRESULT hr11 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mPatrolIconNOtexture.str().c_str(), nullptr, &mPatrolIconNO);
		HRESULT hr12 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mStopIconNOtexture.str().c_str(), nullptr, &mStopIconNO);
		HRESULT hr13 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mStopIconYEStexture.str().c_str(), nullptr, &mStopIconYES);
		HRESULT hr14 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mHideIconNOtexture.str().c_str(), nullptr, &mHideIconNO);
		HRESULT hr15 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mHideIconYEStexture.str().c_str(), nullptr, &mHideIconYES);
		HRESULT hr16 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mNoiseIconNOtexture.str().c_str(), nullptr, &mNoiseIconNO);
		HRESULT hr17 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mNoiseIconYEStexture.str().c_str(), nullptr, &mNoiseIconYES);
		HRESULT hr18 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mMultiSelectionPortraittexture.str().c_str(), nullptr, &mMultiSelectionPortrait);
		HRESULT hr19 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mNoiseIconMAYBEtexture.str().c_str(), nullptr, &mNoiseIconMAYBE);
		HRESULT hr20 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mHideIconMAYBEtexture.str().c_str(), nullptr, &mHideIconMAYBE);
		HRESULT hr21 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mStopIconMAYBEtexture.str().c_str(), nullptr, &mStopIconMAYBE);
		HRESULT hr22 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mPatrolIconMAYBEtexture.str().c_str(), nullptr, &mPatrolIconMAYBE);
		HRESULT hr23 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mCleanIconMAYBEtexture.str().c_str(), nullptr, &mCleanIconMAYBE);
		HRESULT hr24 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mPaintIconMAYBEtexture.str().c_str(), nullptr, &mPaintIconMAYBE);
		HRESULT hr25 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mPaintIconCLICKEDtexture.str().c_str(), nullptr, &mPaintIconCLICKED);
		HRESULT hr26 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mCleanIconCLICKEDtexture.str().c_str(), nullptr, &mCleanIconCLICKED);
		HRESULT hr27 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mPatrolIconCLICKEDtexture.str().c_str(), nullptr, &mPatrolIconCLICKED);
		HRESULT hr28 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mStopIconCLICKEDtexture.str().c_str(), nullptr, &mStopIconCLICKED);
		HRESULT hr29 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mHideIconCLICKEDtexture.str().c_str(), nullptr, &mHideIconCLICKED);
		HRESULT hr30 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mNoiseIconCLICKEDtexture.str().c_str(), nullptr, &mNoiseIconCLICKED);
		HRESULT hr31 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mOptionButtonYEStexture.str().c_str(), nullptr, &mOptionButtonYES);
		HRESULT hr32 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mOptionButtonMAYBEtexture.str().c_str(), nullptr, &mOptionButtonMAYBE);
		HRESULT hr33 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mOptionButtonCLICKEDtexture.str().c_str(), nullptr, &mOptionButtonCLICKED);
		HRESULT hr34 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mOptionButtonNOtexture.str().c_str(), nullptr, &mOptionButtonNO);
		HRESULT hr35 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mTacticalMapTexture.str().c_str(), nullptr, &mTacticalMap);
		HRESULT hr36 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mTacticalButton1NOtexture.str().c_str(), nullptr, &mTacticalButton1NO);
		HRESULT hr37 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mTacticalButton1YEStexture.str().c_str(), nullptr, &mTacticalButton1YES);
		HRESULT hr38 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mTacticalButton1MAYBEtexture.str().c_str(), nullptr, &mTacticalButton1MAYBE);
		HRESULT hr39 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mTacticalButton1CLICKEDtexture.str().c_str(), nullptr, &mTacticalButton1CLICKED);
		HRESULT hr40 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mTacticalButton2NOtexture.str().c_str(), nullptr, &mTacticalButton2NO);
		HRESULT hr41 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mTacticalButton3NOtexture.str().c_str(), nullptr, &mTacticalButton3NO);
		HRESULT hr42 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mStartButtonNOtexture.str().c_str(), nullptr, &mStartButtonNO);
		HRESULT hr43 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mStartButtonMAYBEtexture.str().c_str(), nullptr, &mStartButtonMAYBE);
		HRESULT hr44 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mStartButtonCLICKEDtexture.str().c_str(), nullptr, &mStartButtonCLICKED);
		HRESULT hr45 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mPaintSoldierNOtexture.str().c_str(), nullptr, &mPaintSoldierNO);
		HRESULT hr46 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mPaintSoldierYEStexture.str().c_str(), nullptr, &mPaintSoldierYES);
		HRESULT hr47 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mPaintSoldierMAYBEtexture.str().c_str(), nullptr, &mPaintSoldierMAYBE);
		HRESULT hr48 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mPaintSoldierCLICKEDtexture.str().c_str(), nullptr, &mPaintSoldierCLICKED);
		HRESULT hr49 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mshowPainter.str().c_str(), nullptr, &mPaintCutsceenPortrait1);
		HRESULT hr50= DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mshowPainter1.str().c_str(), nullptr, &mPaintCutsceenPortrait2);
		HRESULT hr51 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mshowPainter2.str().c_str(), nullptr, &mPaintCutsceenPortrait3);
		HRESULT hr52 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mshowPainter3.str().c_str(), nullptr, &mPaintCutsceenPortrait4);
		HRESULT hr53 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mshowPainter3.str().c_str(), nullptr, &mPaintCutsceenPortrait4);
		HRESULT hr54 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mbuttonXNOtex.str().c_str(), nullptr, &mbuttonXNO);
		HRESULT hr55 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mbuttonXMAYBEtex.str().c_str(), nullptr, &mbuttonXMAYBE);
		HRESULT hr56 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mbuttonXCLICKEDtex.str().c_str(), nullptr, &mbuttonXCLICKED);
		HRESULT hr57 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mcloud1texture.str().c_str(), nullptr, &mcloud1);
		HRESULT hr58 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mcloud2texture.str().c_str(), nullptr, &mcloud2);
		HRESULT hr59 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mcloud3texture.str().c_str(), nullptr, &mcloud3);
		HRESULT hr60 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mcloud4texture.str().c_str(), nullptr, &mcloud4);
		HRESULT hr61 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mcloud5texture.str().c_str(), nullptr, &mcloud5);
		HRESULT hr62 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mcloud6texture.str().c_str(), nullptr, &mcloud6);
		HRESULT hr63 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mcloud7texture.str().c_str(), nullptr, &mcloud7);
		HRESULT hr64 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), textureOptionActionsBanner1.str().c_str(), nullptr, &mOptionActionsBanner1);
		HRESULT hr65 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mPatrolModeBannerSoldiertexture.str().c_str(), nullptr, &mPatrolModeBannerSoldier);
		HRESULT hr66 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mPatrolModeBannerPainttexture.str().c_str(), nullptr, &mPatrolModeBannerPaint);
		HRESULT hr67 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mTacticalButton2YEStexture.str().c_str(), nullptr, &mTacticalButton2YES);
		HRESULT hr68 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mTacticalButton2MAYBEtexture.str().c_str(), nullptr, &mTacticalButton2MAYBE);
		HRESULT hr69 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mTacticalButton2CLICKEDtexture.str().c_str(), nullptr, &mTacticalButton2CLICKED);
		HRESULT hr70 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mTacticalButton3CLICKEDtexture.str().c_str(), nullptr, &mTacticalButton3CLICKED);
		HRESULT hr71 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mTacticalButton3MAYBEtexture.str().c_str(), nullptr, &mTacticalButton3MAYBE);
		HRESULT hr72 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), mTacticalButton3YEStexture.str().c_str(), nullptr, &mTacticalButton3YES);

		if (FAILED(hr))
			throw GameException("CreateWICTextureFromFile1() failed.", hr);

		if (FAILED(hr2))
			throw GameException("CreateWICTextureFromFile1(2) failed.", hr);

		if (FAILED(hr3))
			throw GameException("CreateWICTextureFromFile3() failed.", hr3);

		if (FAILED(hr4))
			throw GameException("CreateWICTextureFromFile4() failed.", hr);

		if (FAILED(hr5))
			throw GameException("CreateWICTextureFromFile5() failed.", hr);

		if (FAILED(hr6))
			throw GameException("CreateWICTextureFromFile6() failed.", hr);

		if (FAILED(hr7))
			throw GameException("CreateWICTextureFromFile7() failed.", hr);

		if (FAILED(hr8))
			throw GameException("CreateWICTextureFromFile8() failed.", hr8);

		if (FAILED(hr9))
			throw GameException("CreateWICTextureFromFile9() failed.", hr9);

		if (FAILED(hr10))
			throw GameException("CreateWICTextureFromFile01() failed.", hr);


		if (FAILED(hr11))
			throw GameException("CreateWICTextureFromFile11() failed.", hr);

		if (FAILED(hr12))
			throw GameException("CreateWICTextureFromFile12() failed.", hr);


		if (FAILED(hr13))
			throw GameException("CreateWICTextureFromFile13() failed.", hr);


		if (FAILED(hr14))
			throw GameException("CreateWICTextureFromFile14() failed.", hr);

		if (FAILED(hr18))
			throw GameException("CreateWICTextureFromFile18() failed.", hr);

		if (FAILED(hr15))
			throw GameException("CreateWICTextureFromFile15() failed.", hr);

		if (FAILED(hr16))
			throw GameException("CreateWICTextureFromFile16() failed.", hr);

		if (FAILED(hr17))
			throw GameException("CreateWICTextureFromFile17() failed.", hr);

		if (FAILED(hr18))
			throw GameException("CreateWICTextureFromFile18() failed.", hr18);

		if (FAILED(hr19))
			throw GameException("CreateWICTextureFromFile19() failed.", hr19);

		if (FAILED(hr20))
			throw GameException("CreateWICTextureFromFile20() failed.", hr20);

		if (FAILED(hr21))
			throw GameException("CreateWICTextureFromFile21() failed.", hr21);

		if (FAILED(hr22))
			throw GameException("CreateWICTextureFromFile22() failed.", hr22);

		if (FAILED(hr23))
			throw GameException("CreateWICTextureFromFile23() failed.", hr23);
		
		if (FAILED(hr35))
			throw GameException("CreateWICTextureFromFile1() failed.", hr35);

		if (FAILED(hr35))
			throw GameException("CreateWICTextureFromFile1() failed.", hr35);

		if (FAILED(hr35))
			throw GameException("CreateWICTextureFromFile1() failed.", hr35);

		if (FAILED(hr35))
			throw GameException("CreateWICTextureFromFile1() failed.", hr35);

		if (FAILED(hr45))
			throw GameException("CreateWICTextureFromFile45() failed.", hr34);


		if (FAILED(hr49))
			throw GameException("CreateWICTextureFromFile49() failed.", hr49);
		if (FAILED(hr50))
			throw GameException("CreateWICTextureFromFile50() failed.", hr50);
		if (FAILED(hr51))
			throw GameException("CreateWICTextureFromFile51() failed.", hr51);
		if (FAILED(hr52))
			throw GameException("CreateWICTextureFromFile52() failed.", hr52);
		if (FAILED(hr65))
			throw GameException("CreateWICTextureFromFile65() failed.", hr65);
		if (FAILED(hr66))
			throw GameException("CreateWICTextureFromFile66() failed.", hr66);
		if (FAILED(hr67))
			throw GameException("CreateWICTextureFromFile67() failed.", hr67);
		if (FAILED(hr68))
			throw GameException("CreateWICTextureFromFile68() failed.", hr68);
		if (FAILED(hr69))
			throw GameException("CreateWICTextureFromFile69() failed.", hr69);
		if (FAILED(hr70))
			throw GameException("CreateWICTextureFromFile70() failed.", hr70);
		if (FAILED(hr71))
			throw GameException("CreateWICTextureFromFile71() failed.", hr71);
		if (FAILED(hr72))
			throw GameException("CreateWICTextureFromFile72() failed.", hr72);

		Game::Initialize();
	}

	void RenderingGame::Shutdown()
	{
		DeleteObject(mGameManager);
		DeleteObject(mKeyboard);
		DeleteObject(mMouse);
		DeleteObject(mFpsComponent);
		DeleteObject(mSkybox);
		DeleteObject(mSpriteBatch);
		DeleteObject(mSpriteFont);
		DeleteObject(mCamera);
		DeleteObject(mRenderStateHelper);

		ReleaseObject(mDirectInput);

		Game::Shutdown();
	}

	void RenderingGame::Update(const GameTime & gameTime)
	{

		if (mKeyboard->WasKeyPressedThisFrame(DIK_ESCAPE))
			Exit();

		if (mKeyboard->WasKeyPressedThisFrame(DIK_F1))
			mGameManager->StartScene(MENU_LEVEL);

		if (mKeyboard->WasKeyPressedThisFrame(DIK_F2))
			mGameManager->StartScene(DAY_LEVEL);

		if (mKeyboard->WasKeyPressedThisFrame(DIK_F3))
			mGameManager->StartScene(TRAIN_LEVEL);

		if (mKeyboard->WasKeyPressedThisFrame(DIK_F4))
			mGameManager->StartScene(CITY_LEVEL);

		if (mKeyboard->WasKeyPressedThisFrame(DIK_F5))
			mGameManager->StartScene(CREATION_KIT_LEVEL);

		if(mKeyboard->WasKeyPressedThisFrame(DIK_F6))
			mGameManager->StartScene(PATHFINDER_TEST);	

		// Units
		if (mKeyboard->WasKeyPressedThisFrame(DIK_1) && mGameManager->GetListOfUnits().size() >= 1)
			keybordButtonSelectUnit = 0;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_2) && mGameManager->GetListOfUnits().size() >= 2)
			keybordButtonSelectUnit = 1;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_3) && mGameManager->GetListOfUnits().size() >= 3)
			keybordButtonSelectUnit = 2;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_4) && mGameManager->GetListOfUnits().size() >= 4)
			keybordButtonSelectUnit = 3;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_5) && mGameManager->GetListOfUnits().size() >= 5)
			keybordButtonSelectUnit = 4;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_6) && mGameManager->GetListOfUnits().size() >= 6)
			keybordButtonSelectUnit = 5;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_7) && mGameManager->GetListOfUnits().size() >= 7)
			keybordButtonSelectUnit = 6;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_8) && mGameManager->GetListOfUnits().size() >= 8)
			keybordButtonSelectUnit = 7;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_9) && mGameManager->GetListOfUnits().size() >= 9)
			keybordButtonSelectUnit = 8;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_0) && mGameManager->GetListOfUnits().size() >= 10)
			keybordButtonSelectUnit = 9;

		if (mKeyboard->WasKeyPressedThisFrame(DIK_TAB) && showFarbaManGUI)
		{
			FarbaManGUISelected = !FarbaManGUISelected;
			keybordButtonSelectUnit = FARBA_MAN_ID;
		}


		if (!showFarbaManGUI) {
			if (mGameManager->GetrenderGameFarbaManSpawnFlag()) {
				showFarbaManGUI = true;
			}
		}
		
		float diffBetweenUnitsIcons = (250.0f / mGameManager->GetListOfUnits().size());
		
	
		if (showUnitsGui == false) {
			if (mMouse->X() > 320.0f  && mMouse->X() < 750.0f && mMouse->Y() > 680.0f) {
				showUnitsGui = true;
			}
		}
		else if (showUnitsGui == true) {
			if (mMouse->X() < 320.0f  || mMouse->X() > 750.0f || mMouse->Y() < 570.0f) {
				showUnitsGui = false;
			}
		}
		
		


		if (mMouse->Y() > 640.0f && mMouse->X() > 20.0f  && mMouse->X() < 369.0f) {

		}
		else if (mMouse->WasButtonPressedThisFrame(MouseButtonsLeft))
		{
			timeFromPressed = gameTime;
			//mouse1Pos = XMFLOAT2({ mMouse->X(), mMouse->Y()});
			mouse1Pos.x = 0.0f + mMouse->X();
			mouse1Pos.y = 0.0f + mMouse->Y();
			selectedOnce = false;
		}
		//else if (mMouse->IsButtonHeldDown(MouseButtonsLeft) && mKeyboard->IsKeyDown(DIK_LCONTROL))
		else if (mMouse->IsButtonHeldDown(MouseButtonsLeft))
		{
			if (mMouse->X() < 720.0f && mMouse->Y()<560.0f) {
				if (gameTime.TotalGameTime() - timeFromPressed.TotalGameTime() >= 0.3)
				{
					mouse2Pos.x = 0.0f + mMouse->X();
					mouse2Pos.y = 0.0f + mMouse->Y();
					selectedOnce = true;
				}
			}
			
		}
		else if (mMouse->WasButtonReleasedThisFrame(MouseButtonsLeft))
		{
			if (!(mMouse->Y() > 610.0f && mMouse->X() > 330.0f )) {
				if (selectedOnce)
				{
					mGameManager->SelectingUnits(mouse1Pos.x, mouse1Pos.y, mouse2Pos.x, mouse2Pos.y);
					selectedOnce = false;
				}
				else  
					mGameManager->SelectingUnits(mouse1Pos.x, mouse1Pos.y);
			}
			
		}

		if (mMouse->WasButtonPressedThisFrame(MouseButtonsRight))
		{
			if (mMouse->Y() > 640.0f && mMouse->X() > 20.0f  && mMouse->X() < 369.0f) {

			}
			else {
				if(!patrolMode)
					mGameManager->SelectingGrounds(mMouse->X(), mMouse->Y());
				else {
					mGameManager->SelectingGroundsFakePatrolMode(mMouse->X(), mMouse->Y());
				}
			}

		
		}



		Game::Update(gameTime);
	}

	void RenderingGame::Draw(const GameTime & gameTime)
	{
		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView,
			reinterpret_cast<const float*>(&BackgroundColor));

		mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView,
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		
		Game::Draw(gameTime);
		
		static auto m_states = std::make_unique<CommonStates>(Direct3DDevice());
		mRenderStateHelper->SaveAll();
		mSpriteBatch->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
		int posOffset = 0;
		std::wostringstream mouseLabel;
		//mouseLabel << L"Mouse Position: " << mMouse->X() << ", "
			//<< mMouse->Y() << " Mouse Wheel: " << mMouse->Wheel();
		mSpriteFont->DrawString(mSpriteBatch, mouseLabel.str().c_str(), mMouseTextPosition, Colors::White);
		if (mapLevel == true) {

			if (showMapVal) {
				mSpriteBatch->Draw(mTacticalMap, SimpleMath::Rectangle(0, 0, 1028.0f, 780.0f));
				
			}

			if (cutSceneId == 6) {
				mSpriteBatch->Draw(mPaintCutsceenPortrait1, SimpleMath::Rectangle(700.0f, 150.0f, 350.0f, 300.0f));
				mSpriteBatch->Draw(mcloud7, SimpleMath::Rectangle(700.0f, 15.0f, 300.0f, 200.0f));
				mSpriteBatch->Draw(mbuttonXNO, SimpleMath::Rectangle(700.0f, 150.0f, 30.0f, 30.0f));
				if (mMouse->X() > 700.0f && mMouse->X() < 730.0f  && mMouse->Y() > 150.0f  && mMouse->Y() < 180.0f) {
					mSpriteBatch->Draw(mbuttonXMAYBE, SimpleMath::Rectangle(700.0f, 150.0f, 30.0f, 30.0f));
					if (mMouse->WasButtonReleasedThisFrame(MouseButtonsLeft)) {
						mSpriteBatch->Draw(mbuttonXCLICKED, SimpleMath::Rectangle(700.0f, 150.0f, 30.0f, 30.0f));
						cutSceneId = -1;
						mapLevel = false;
					}
				}
			}

			
			if (cutSceneId == 5) {
				mSpriteBatch->Draw(mPaintCutsceenPortrait3, SimpleMath::Rectangle(700.0f, 150.0f, 350.0f, 300.0f));
				mSpriteBatch->Draw(mcloud6, SimpleMath::Rectangle(700.0f, 15.0f, 300.0f, 200.0f));
				mSpriteBatch->Draw(mbuttonXNO, SimpleMath::Rectangle(700.0f, 150.0f, 30.0f, 30.0f));
				if (mMouse->X() > 700.0f && mMouse->X() < 730.0f  && mMouse->Y() > 150.0f  && mMouse->Y() < 180.0f) {
					mSpriteBatch->Draw(mbuttonXMAYBE, SimpleMath::Rectangle(700.0f, 150.0f, 30.0f, 30.0f));
					if (mMouse->WasButtonReleasedThisFrame(MouseButtonsLeft)) {
						mSpriteBatch->Draw(mbuttonXCLICKED, SimpleMath::Rectangle(700.0f, 150.0f, 30.0f, 30.0f));
						cutSceneId = -1;
						mapLevel = false;
					}
				}
			}

			if (cutSceneId == 4) {
				mSpriteBatch->Draw(mPaintCutsceenPortrait3, SimpleMath::Rectangle(700.0f, 150.0f, 350.0f, 300.0f));
				mSpriteBatch->Draw(mcloud5, SimpleMath::Rectangle(700.0f, 15.0f, 300.0f, 200.0f));
				mSpriteBatch->Draw(mbuttonXNO, SimpleMath::Rectangle(700.0f, 150.0f, 30.0f, 30.0f));
				if (mMouse->X() > 700.0f && mMouse->X() < 730.0f  && mMouse->Y() > 150.0f  && mMouse->Y() < 180.0f) {
					mSpriteBatch->Draw(mbuttonXMAYBE, SimpleMath::Rectangle(700.0f, 150.0f, 30.0f, 30.0f));
					if (mMouse->WasButtonReleasedThisFrame(MouseButtonsLeft)) {
						mSpriteBatch->Draw(mbuttonXCLICKED, SimpleMath::Rectangle(700.0f, 150.0f, 30.0f, 30.0f));
						cutSceneId = -1;
						mapLevel = false;
					}
				}
			}

			if (cutSceneId == 3) {
				mSpriteBatch->Draw(mPaintCutsceenPortrait4, SimpleMath::Rectangle(700.0f, 470.0f, 350.0f, 300.0f));
				mSpriteBatch->Draw(mcloud4, SimpleMath::Rectangle(600.0f, 335.0f, 300.0f, 200.0f));
				mSpriteBatch->Draw(mbuttonXNO, SimpleMath::Rectangle(580.0f, 490.0f, 30.0f, 30.0f));
				if (mMouse->X() > 580.0f && mMouse->X() < 610.0f  && mMouse->Y() > 490.0f  && mMouse->Y() < 520.0f) {
					mSpriteBatch->Draw(mbuttonXMAYBE, SimpleMath::Rectangle(580.0f, 490.0f, 30.0f, 30.0f));
					if (mMouse->WasButtonReleasedThisFrame(MouseButtonsLeft)) {
						mSpriteBatch->Draw(mbuttonXCLICKED, SimpleMath::Rectangle(580.0f, 490.0f, 30.0f, 30.0f));
						cutSceneId = -1;
					}
				}
			}

			if (cutSceneId == 2) {
				mSpriteBatch->Draw(mPaintCutsceenPortrait3, SimpleMath::Rectangle(700.0f, 470.0f, 350.0f, 300.0f));
				mSpriteBatch->Draw(mcloud3, SimpleMath::Rectangle(700.0f, 335.0f, 300.0f, 200.0f));
				mSpriteBatch->Draw(mbuttonXNO, SimpleMath::Rectangle(700.0f, 470.0f, 30.0f, 30.0f));
				if (mMouse->X() > 700.0f && mMouse->X() < 730.0f  && mMouse->Y() > 470.0f  && mMouse->Y() < 500.0f) {
					mSpriteBatch->Draw(mbuttonXMAYBE, SimpleMath::Rectangle(700.0f, 470.0f, 30.0f, 30.0f));
					if (mMouse->WasButtonReleasedThisFrame(MouseButtonsLeft)) {
						mSpriteBatch->Draw(mbuttonXCLICKED, SimpleMath::Rectangle(700.0f, 470.0f, 30.0f, 30.0f));
						cutSceneId = 3;
					}
				}
			}

			if (cutSceneId == 1) {
				mSpriteBatch->Draw(mPaintCutsceenPortrait1, SimpleMath::Rectangle(700.0f, 470.0f, 350.0f, 300.0f));
				mSpriteBatch->Draw(mcloud2, SimpleMath::Rectangle(600.0f, 335.0f, 300.0f, 200.0f));
				mSpriteBatch->Draw(mbuttonXNO, SimpleMath::Rectangle(580.0f, 490.0f, 30.0f, 30.0f));
				if (mMouse->X() > 580.0f && mMouse->X() < 610.0f  && mMouse->Y() > 490.0f  && mMouse->Y() < 520.0f) {
					mSpriteBatch->Draw(mbuttonXMAYBE, SimpleMath::Rectangle(580.0f, 490.0f, 30.0f, 30.0f));
					if (mMouse->WasButtonReleasedThisFrame(MouseButtonsLeft)) {
						mSpriteBatch->Draw(mbuttonXCLICKED, SimpleMath::Rectangle(580.0f, 490.0f, 30.0f, 30.0f));
						cutSceneId = 2;
					}
				}
			}

			if (cutSceneId == 0) {
				mSpriteBatch->Draw(mPaintCutsceenPortrait2, SimpleMath::Rectangle(700.0f, 470.0f, 350.0f, 300.0f));
				mSpriteBatch->Draw(mcloud1, SimpleMath::Rectangle(700.0f, 335.0f, 300.0f, 200.0f));
				mSpriteBatch->Draw(mbuttonXNO, SimpleMath::Rectangle(700.0f, 470.0f, 30.0f, 30.0f));
				if (mMouse->X() > 700.0f && mMouse->X() < 730.0f  && mMouse->Y() > 470.0f  && mMouse->Y() < 500.0f) {
					mSpriteBatch->Draw(mbuttonXMAYBE, SimpleMath::Rectangle(700.0f, 470.0f, 30.0f, 30.0f));
					if (mMouse->WasButtonReleasedThisFrame(MouseButtonsLeft)) {
						mSpriteBatch->Draw(mbuttonXCLICKED, SimpleMath::Rectangle(700.0f, 470.0f, 30.0f, 30.0f));
						cutSceneId = 1;
					}
				}
			}

			
			
			if(showMapVal)
			mSpriteBatch->Draw(mTacticalButton1NO, SimpleMath::Rectangle(500.0f, 650.0f, 80.0f, 80.0f));

			if (mMouse->X() > 515.0f && mMouse->X() < 570.0f  && mMouse->Y() > 665.0f  && mMouse->Y() < 720.0f) {
				mSpriteBatch->Draw(mTacticalButton1CLICKED, SimpleMath::Rectangle(500.0f, 650.0f, 80.0f, 80.0f));
				if (mMouse->IsButtonHeldDown(MouseButtonsLeft)) {
					mSpriteBatch->Draw(mTacticalButton1MAYBE, SimpleMath::Rectangle(500.0f, 650.0f, 80.0f, 80.0f));
					whichTacticalMapButtonIsClicking = 1;
				}
				
			}

			if (showMapVal) {
				mSpriteBatch->Draw(mTacticalButton2NO, SimpleMath::Rectangle(60.0f, 300.0f, 80.0f, 80.0f));

				if (mMouse->X() > 60.0f && mMouse->X() < 130.0f  && mMouse->Y() > 305.0f  && mMouse->Y() < 370.0f) {
					mSpriteBatch->Draw(mTacticalButton2CLICKED, SimpleMath::Rectangle(60.0f, 300.0f, 80.0f, 80.0f));
					if (mMouse->IsButtonHeldDown(MouseButtonsLeft)) {
						mSpriteBatch->Draw(mTacticalButton2MAYBE, SimpleMath::Rectangle(60.0f, 300.0f, 80.0f, 80.0f));
						whichTacticalMapButtonIsClicking = 2;
					}
				}

				mSpriteBatch->Draw(mTacticalButton3NO, SimpleMath::Rectangle(400.0f, 40.0f, 80.0f, 80.0f));
				if (mMouse->X() > 400.0f && mMouse->X() < 470.0f  && mMouse->Y() > 50.0f  && mMouse->Y() < 110.0f) {
					mSpriteBatch->Draw(mTacticalButton3CLICKED, SimpleMath::Rectangle(400.0f, 40.0f, 80.0f, 80.0f));
					if (mMouse->IsButtonHeldDown(MouseButtonsLeft)) {
						mSpriteBatch->Draw(mTacticalButton3MAYBE, SimpleMath::Rectangle(400.0f, 40.0f, 80.0f, 80.0f));
						whichTacticalMapButtonIsClicking = 3;
					}
				}
				
			}
			

			if ((whichTacticalMapButtonIsClicking == 1  || whichTacticalMapButtonIsClicking == 2 || whichTacticalMapButtonIsClicking == 3) && showMapVal) {
				if(whichTacticalMapButtonIsClicking == 1)
					mSpriteBatch->Draw(mTacticalButton1YES, SimpleMath::Rectangle(500.0f, 650.0f, 80.0f, 80.0f));

				if (whichTacticalMapButtonIsClicking == 2)
					mSpriteBatch->Draw(mTacticalButton2YES, SimpleMath::Rectangle(60.0f, 300.0f, 80.0f, 80.0f));

				if (whichTacticalMapButtonIsClicking == 3)
					mSpriteBatch->Draw(mTacticalButton3YES, SimpleMath::Rectangle(400.0f, 40.0f, 80.0f, 80.0f));

				mSpriteBatch->Draw(mStartButtonNO, SimpleMath::Rectangle(700.0f, 640.0f, 300.0f, 100.0f));
				if (mMouse->X() > 700.0f && mMouse->X() < 1000.0f  && mMouse->Y() > 645.0f  && mMouse->Y() < 725.0f) {
					mSpriteBatch->Draw(mStartButtonMAYBE, SimpleMath::Rectangle(700.0f, 640.0f, 300.0f, 100.0f));
					if (mMouse->IsButtonHeldDown(MouseButtonsLeft)) {
						mSpriteBatch->Draw(mStartButtonCLICKED, SimpleMath::Rectangle(700.0f, 640.0f, 300.0f, 100.0f));
						buttonWasHold = true;
					}
					if (mMouse->WasButtonReleasedThisFrame(MouseButtonsLeft) && buttonWasHold) {
						mapLevel = false;
						buttonWasHold = false;
						cutSceneId = -1;
						showMapVal = false;
						gameLevel = true;
						mGameManager->StartScene(TRAIN_LEVEL);
					}
				}
			}
		}
	

		if(gameLevel) {
			if (showUnitsGui || !(indexSelectedGuiButtons.empty())) {
				//mSpriteBatch->Draw(mUnitsBanner, SimpleMath::Rectangle(320.0f, 570.0f, 400.0f, 220.0f));
				posOffset = 1;

			}
			else if (!showUnitsGui)
			{
				//mSpriteBatch->Draw(mUnitsBanner, SimpleMath::Rectangle(320.0f, 650.0f, 400.0f, 220.0f));
				posOffset = 0;
			}

			if (showFarbaManGUI) {
				mSpriteBatch->Draw(mPaintSoldierNO, SimpleMath::Rectangle(300.f, 710.0f - posOffset * 60, 150.0f, 110.0f));
				if (FarbaManGUISelected || (mMouse->X()>340 && mMouse->X()<395 && mMouse->Y()>650 && mMouse->Y()<760)) {
					mSpriteBatch->Draw(mPaintSoldierMAYBE, SimpleMath::Rectangle(300.f, 710.0f - posOffset * 60, 150.0f, 110.0f));
					if (FarbaManGUISelected || mMouse->WasButtonReleasedThisFrame(MouseButtonsLeft)) {
							
						
						
						mSpriteBatch->Draw(mPaintSoldierYES, SimpleMath::Rectangle(300.f, 710.0f - posOffset * 60, 150.0f, 110.0f));
					}
				}
				if (farbaManCutScene4Flag) {
					cutSceneId = 4;
					mapLevel = true;
					farbaManCutScene4Flag = false;
				}
				
			}

			if (true)
			{
				for (int i = 0; i < mGameManager->GetListOfUnits().size(); i++)
				{

					if (mGameManager->GetListOfUnits().at(i)->GetUnitID() == mGameManager->unitID) {
						//mSpriteBatch->Draw(mGreenSoldierNO, SimpleMath::Rectangle(360.f + 30 * i, 630.0f - posOffset * 80, 150.0f, 100.0f));
					}
						

					else
					{
						if (mMouse->X() > (395 + i * 40) && (mMouse->X() < (395 + i * 40 + 40) && mMouse->Y() > 625) || keybordButtonSelectUnit == i || keybordButtonSelectUnit==100)		//uzywane dla zaznaczenie farbamana, on jest zawsze tylko jeden wiec nie ma problemu z nadaniem mu stalej wartosci, a mniej roboty
						{
							
							mSpriteBatch->Draw(mGreenSoldierMAYBE, SimpleMath::Rectangle(360.f + 40 * i, 700.0f - posOffset *60, 150.0f, 120.0f));
				

							if (mMouse->WasButtonReleasedThisFrame(MouseButtonsLeft) || keybordButtonSelectUnit == i || keybordButtonSelectUnit == 100) {
								showUnitDetail = true;
								bool checkCopy = false;


								if(keybordButtonSelectUnit!=100){
									keybordButtonSelectUnit = -1;
									for (int x = 0; x < indexSelectedGuiButtons.size(); x++) {	//sprawdzanie czy juz raz kliknieto na ikone, jesli tak to nie zapisuj do wektora indeksow zaznaczonych drugi raz
										if (indexSelectedGuiButtons.at(x) == i) {
											checkCopy = true;
										}
									}
									if (!checkCopy) {
										indexSelectedGuiButtons.push_back(i);
										GreenSoldier* greenSold = mGameManager->GetListOfUnits().at(i)->As<GreenSoldier>();
										greenSold->setSelection(true);
										greenSold->setIsSelected(true);								//zaznaczanie jednostek w grze przez gui
									}
									else {
										std::vector<int> buffor;
										buffor.clear();
										for (int z = 0; z < indexSelectedGuiButtons.size(); z++) {
											if (indexSelectedGuiButtons.at(z) != i) {
												buffor.push_back(indexSelectedGuiButtons.at(z));
											}
										}
										indexSelectedGuiButtons.clear();
										indexSelectedGuiButtons = buffor;
										GreenSoldier* greenSold = mGameManager->GetListOfUnits().at(i)->As<GreenSoldier>();
										greenSold->setSelection(false);
										greenSold->setIsSelected(false);
									}
								}
								
								else {
									keybordButtonSelectUnit = -1;
									FarbaMan* farbaM = mGameManager->GetListOfUnits().at(5)->As<FarbaMan>();
									if (!FarbaManSelectedFlagOnce) {
										farbaM->setSelection(true);
										farbaM->setIsSelected(true);
										FarbaManSelectedFlagOnce = true;
									}
									else {
										farbaM->setSelection(false);
										farbaM->setIsSelected(false);
										FarbaManSelectedFlagOnce = false;
									}
									
								}
								
							}
						}

						else 
						{
							mSpriteBatch->Draw(mGreenSoldierNO, SimpleMath::Rectangle(360.f + 40 * i, 700.0f - posOffset * 60, 150.0f, 120.0f));

						}
					}
				}
			}

			if (!(indexSelectedGuiButtons.empty())) {
				for (int i = 0; i < indexSelectedGuiButtons.size(); i++) {
					mSpriteBatch->Draw(mGreenSoldierYES, SimpleMath::Rectangle(360.f + 40 * indexSelectedGuiButtons.at(i), 700.0f - posOffset * 60, 150.0f, 120.0f));
				}
			}
			else {
				showUnitDetail = false;
				//farbaManButton 
				if (FarbaManGUISelected) {
					mSpriteBatch->Draw(mOptionActionsBanner1, SimpleMath::Rectangle(720.0f, 560.0f, 300.0f, 250.0f));

					float a = mMouse->X();
					
					if (mMouse->X() > 750.0f && mMouse->X() < 850.0f  && mMouse->Y() > 600.0f  && mMouse->Y() < 685.0f) 
					{
						mSpriteBatch->Draw(mPaintIconMAYBE, SimpleMath::Rectangle(750.0f, 595.0f, 100.0f, 100.0f));
						if (mMouse->IsButtonHeldDown(MouseButtonsLeft)) {
							if (mGameManager->GetPaintButtonFlag()) {
								paintMode = true;
							}
							else {
								mSpriteBatch->Draw(mPaintIconNO, SimpleMath::Rectangle(750.0f, 595.0f, 100.0f, 100.0f));
							}
						}
					}
					
					else {
						mSpriteBatch->Draw(mPaintIconYES, SimpleMath::Rectangle(750.0f, 595.0f, 100.0f, 100.0f));
					}

					if (paintMode) {
						mSpriteBatch->Draw(mPaintIconCLICKED, SimpleMath::Rectangle(750.0f, 595.0f, 100.0f, 100.0f));
						if (paintMode1flag) {
							PaintMode1();
						}
						if (mGameManager->GetPaintButtonFlag()) {
							paintMode = false;
							cutSceneId = 6;
							mapLevel = true;
						}
						
					}

					if (mMouse->X() > 870.0f && mMouse->X() < 970.0f  && mMouse->Y() > 600.0f  && mMouse->Y() < 685.0f) {
						mSpriteBatch->Draw(mPatrolIconMAYBE, SimpleMath::Rectangle(870.0f, 592.0f, 100.0f, 100.0f));
						if (mMouse->WasButtonPressedThisFrame(MouseButtonsLeft)) {
							if (!patrolMode) {
								patrolMode = true;
							}
							else {
								patrolMode = false;
							}
							
						}
					}

					else
						mSpriteBatch->Draw(mPatrolIconYES, SimpleMath::Rectangle(870.0f, 592.0f, 100.0f, 100.0f));

					if (patrolMode) {
						mSpriteBatch->Draw(mPatrolIconCLICKED, SimpleMath::Rectangle(870.0f, 592.0f, 100.0f, 100.0f));
						mSpriteBatch->Draw(mPatrolModeBannerPaint, SimpleMath::Rectangle(375.0f, 0.0f, 300.0f, 150.0f));
						mSpriteBatch->Draw(mbuttonXNO, SimpleMath::Rectangle(620.0f, 15.0f, 30.0f, 30.0f));
						if (mMouse->X() > 620.0f && mMouse->X() < 650.0f  && mMouse->Y() > 15.0f  && mMouse->Y() < 45.0f) {
							mSpriteBatch->Draw(mbuttonXMAYBE, SimpleMath::Rectangle(620.0f, 15.0f, 30.0f, 30.0f));
							if (mMouse->WasButtonReleasedThisFrame(MouseButtonsLeft)) {
								mSpriteBatch->Draw(mbuttonXCLICKED, SimpleMath::Rectangle(620.0f, 15.0f, 30.0f, 30.0f));
								patrolMode = false;

								mGameManager->ExecutePatrolMode();
							}
						}
					}
					

					if (mMouse->X() > 750.0f && mMouse->X() < 850.0f  && mMouse->Y() > 690.0f  && mMouse->Y() < 790.0f) {
						mSpriteBatch->Draw(mStopIconMAYBE, SimpleMath::Rectangle(750.0f, 690.0f, 100.0f, 100.0f));
						if (mMouse->IsButtonHeldDown(MouseButtonsLeft)) {
							mSpriteBatch->Draw(mStopIconCLICKED, SimpleMath::Rectangle(750.0f, 690.0f, 100.0f, 100.0f));
						}
					}

					else
						mSpriteBatch->Draw(mStopIconYES, SimpleMath::Rectangle(750.0f, 690.0f, 100.0f, 100.0f));

					if (mMouse->X() > 870.0f && mMouse->X() < 970.0f  && mMouse->Y() > 680.0f  && mMouse->Y() < 780.0f) {
						mSpriteBatch->Draw(mHideIconMAYBE, SimpleMath::Rectangle(870.0f, 680.0f, 100.0f, 100.0f));
						if (mMouse->WasButtonReleasedThisFrame(MouseButtonsLeft)) {
							if (!hideMode) {
								hideMode = true;
							}
							else {
								hideMode = false;
							}
						}
					}

					else
						mSpriteBatch->Draw(mHideIconYES, SimpleMath::Rectangle(870.0f, 680.0f, 100.0f, 100.0f));

					if (hideMode) {
						mSpriteBatch->Draw(mHideIconCLICKED, SimpleMath::Rectangle(870.0f, 680.0f, 100.0f, 100.0f));
					}
				}
			}

			//menu button
			if (mMouse->X() > 0.0f && mMouse->X() < 35.0f  && mMouse->Y() > 0.0f  && mMouse->Y() < 35.0f) {
				mSpriteBatch->Draw(mOptionButtonMAYBE, SimpleMath::Rectangle(0, 0, 50.0f, 50.0f));
				if (mMouse->IsButtonHeldDown(MouseButtonsLeft)) {
					mSpriteBatch->Draw(mOptionButtonCLICKED, SimpleMath::Rectangle(0, 0, 50.0f, 50.0f));
				}
			}
			else {
				mSpriteBatch->Draw(mOptionButtonYES, SimpleMath::Rectangle(0, 0, 35.0f, 35.0f));
			}

			if (showUnitDetail == true) {


				//mSpriteBatch->Draw(mPortraitBanner, SimpleMath::Rectangle(765.0f, 330.0f, 250.0f, 250.0f), Colors::White);
				

				if (indexSelectedGuiButtons.size() > 1) {
					//mSpriteBatch->Draw(mMultiSelectionPortrait, SimpleMath::Rectangle(800, 350, 200.0f, 200.0f));
					//mSpriteBatch->Draw(mPaintIconNO, SimpleMath::Rectangle(750.0f, 600.0f, 70.0f, 70.0f));
					//mSpriteBatch->Draw(mCleanIconNO, SimpleMath::Rectangle(830.0f, 597.0f, 80.0f, 80.0f));
					/*if (mMouse->X() > 925.0f && mMouse->X() < 995.0f  && mMouse->Y() > 605.0f  && mMouse->Y() < 665.0f) {
						mSpriteBatch->Draw(mPatrolIconMAYBE, SimpleMath::Rectangle(920.0f, 595.0f, 80.0f, 80.0f));
						if (mMouse->IsButtonHeldDown(MouseButtonsLeft)) {
							mSpriteBatch->Draw(mPatrolIconCLICKED, SimpleMath::Rectangle(920.0f, 595.0f, 80.0f, 80.0f));
						}
					}*/

					//else
					//	mSpriteBatch->Draw(mPatrolIconYES, SimpleMath::Rectangle(920.0f, 595.0f, 80.0f, 80.0f));

					/*if (mMouse->X() > 750.0f && mMouse->X() < 815.0f  && mMouse->Y() > 680.0f  && mMouse->Y() < 745.0f) {
						mSpriteBatch->Draw(mHideIconMAYBE, SimpleMath::Rectangle(740.0f, 670.0f, 90.0f, 90.0f));
						if (mMouse->IsButtonHeldDown(MouseButtonsLeft)) {
							mSpriteBatch->Draw(mHideIconCLICKED, SimpleMath::Rectangle(740.0f, 670.0f, 90.0f, 90.0f));
						}
					}*/

					/*else
						mSpriteBatch->Draw(mHideIconYES, SimpleMath::Rectangle(740.0f, 670.0f, 90.0f, 90.0f));*/

					/*if (mMouse->X() > 840.0f && mMouse->X() < 905.0f  && mMouse->Y() > 680.0f  && mMouse->Y() < 745.0f)
					{
						mSpriteBatch->Draw(mStopIconMAYBE, SimpleMath::Rectangle(830.0f, 680.0f, 90.0f, 90.0f));
						if (mMouse->IsButtonHeldDown(MouseButtonsLeft)) {
							mSpriteBatch->Draw(mStopIconCLICKED, SimpleMath::Rectangle(830.0f, 680.0f, 90.0f, 90.0f));
						}
					}

					else
						mSpriteBatch->Draw(mStopIconYES, SimpleMath::Rectangle(830.0f, 680.0f, 90.0f, 90.0f));

					if (mMouse->X() > 925.0f && mMouse->X() < 995.0f  && mMouse->Y() > 680.0f  && mMouse->Y() < 745.0f) {
						mSpriteBatch->Draw(mNoiseIconMAYBE, SimpleMath::Rectangle(920.0f, 680.0f, 85.0f, 85.0f));
						if (mMouse->IsButtonHeldDown(MouseButtonsLeft)) {
							mSpriteBatch->Draw(mNoiseIconCLICKED, SimpleMath::Rectangle(920.0f, 680.0f, 85.0f, 85.0f));
						}
					}

					else
						mSpriteBatch->Draw(mNoiseIconYES, SimpleMath::Rectangle(920.0f, 680.0f, 85.0f, 85.0f));*/
				}
				else {
						//soldierBUTTONS
					if (!FarbaManGUISelected) {
						mSpriteBatch->Draw(mOptionActionsBanner, SimpleMath::Rectangle(720.0f, 560.0f, 300.0f, 250.0f));

						if (mMouse->X() > 750.0f && mMouse->X() < 850.0f  && mMouse->Y() > 600.0f  && mMouse->Y() < 685.0f) {
							mSpriteBatch->Draw(mCleanIconMAYBE, SimpleMath::Rectangle(750.0f, 595.0f, 100.0f, 100.0f));
							if (mMouse->WasButtonPressedThisFrame(MouseButtonsLeft)) {
								if (mGameManager->CheckClearFlag()) {
									clearMode = true;
								}
								else {
									clearMode = false;
								}
							}
								else {
									mSpriteBatch->Draw(mCleanIconNO, SimpleMath::Rectangle(750.0f, 595.0f, 100.0f, 100.0f));
								}
							
						}
						else {
							mSpriteBatch->Draw(mCleanIconYES, SimpleMath::Rectangle(750.0f, 595.0f, 100.0f, 100.0f));
						}

						if (clearMode) {
							mSpriteBatch->Draw(mCleanIconCLICKED, SimpleMath::Rectangle(750.0f, 595.0f, 100.0f, 100.0f));
							ClearMode1();
							clearMode = false;
						}


						if (mMouse->X() > 870.0f && mMouse->X() < 970.0f  && mMouse->Y() > 600.0f  && mMouse->Y() < 685.0f) {
							mSpriteBatch->Draw(mPatrolIconMAYBE, SimpleMath::Rectangle(870.0f, 592.0f, 100.0f, 100.0f));
							if (mMouse->WasButtonPressedThisFrame(MouseButtonsLeft)) {
								if (!patrolMode) {
									patrolMode = true;
								}
								else {
									patrolMode = false;
								}
								
							}
						}

						else {
							mSpriteBatch->Draw(mPatrolIconYES, SimpleMath::Rectangle(870.0f, 592.0f, 100.0f, 100.0f));
						}
							
						if (patrolMode) {
							mSpriteBatch->Draw(mPatrolIconCLICKED, SimpleMath::Rectangle(870.0f, 592.0f, 100.0f, 100.0f));
							mSpriteBatch->Draw(mPatrolModeBannerSoldier, SimpleMath::Rectangle(375.0f, 0.0f, 300.0f, 150.0f));
							mSpriteBatch->Draw(mbuttonXNO, SimpleMath::Rectangle(620.0f, 15.0f, 30.0f, 30.0f));
							if (mMouse->X() > 620.0f && mMouse->X() < 650.0f  && mMouse->Y() > 15.0f  && mMouse->Y() < 45.0f) {
								mSpriteBatch->Draw(mbuttonXMAYBE, SimpleMath::Rectangle(620.0f, 15.0f, 30.0f, 30.0f));
								if (mMouse->WasButtonReleasedThisFrame(MouseButtonsLeft)) {
									mSpriteBatch->Draw(mbuttonXCLICKED, SimpleMath::Rectangle(620.0f, 15.0f, 30.0f, 30.0f));
									patrolMode = false;
									
									mGameManager->ExecutePatrolMode();
									
								}
							}
						}

						if (mMouse->X() > 750.0f && mMouse->X() < 850.0f  && mMouse->Y() > 690.0f  && mMouse->Y() < 790.0f) {
							mSpriteBatch->Draw(mStopIconMAYBE, SimpleMath::Rectangle(750.0f, 690.0f, 100.0f, 100.0f));
							if (mMouse->IsButtonHeldDown(MouseButtonsLeft)) {
								mSpriteBatch->Draw(mStopIconCLICKED, SimpleMath::Rectangle(750.0f, 690.0f, 100.0f, 100.0f));
							}
						}

						else
							mSpriteBatch->Draw(mStopIconYES, SimpleMath::Rectangle(750.0f, 690.0f, 100.0f, 100.0f));

						if (mMouse->X() > 870.0f && mMouse->X() < 970.0f  && mMouse->Y() > 680.0f  && mMouse->Y() < 780.0f) {
							mSpriteBatch->Draw(mHideIconMAYBE, SimpleMath::Rectangle(870.0f, 680.0f, 100.0f, 100.0f));
							if (mMouse->WasButtonReleasedThisFrame(MouseButtonsLeft)) {
								if (!hideMode) {
									hideMode = true;
								}
								else {
									hideMode = false;
								}
							}
						}

						else
							mSpriteBatch->Draw(mHideIconYES, SimpleMath::Rectangle(870.0f, 680.0f, 100.0f, 100.0f));

						if (hideMode) {
							mSpriteBatch->Draw(mHideIconCLICKED, SimpleMath::Rectangle(870.0f, 680.0f, 100.0f, 100.0f));
						}

					}

				}

			}




			if (mGameManager->GetunitsReadyToMove()) {
				std::wostringstream tmp;
				tmp << "GroundActive";
				mSpriteFont->DrawString(mSpriteBatch, tmp.str().c_str(), XMFLOAT2(25.0f, 200.0f), Colors::Blue);
			}

			if (selectedOnce)
			{
				float x1, x2, y1, y2;
				if (mouse1Pos.x > mouse2Pos.x)
				{
					x1 = mouse2Pos.x;
					x2 = mouse1Pos.x;
				}
				else
				{
					x1 = mouse1Pos.x;
					x2 = mouse2Pos.x;
				}

				if (mouse1Pos.y > mouse2Pos.y)
				{
					y1 = mouse2Pos.y;
					y2 = mouse1Pos.y;
				}
				else
				{
					y1 = mouse1Pos.y;
					y2 = mouse2Pos.y;
				}

				mSpriteBatch->Draw(mSelectionRectangleTexture, SimpleMath::Rectangle(x1, y1, abs(x2 - x1), 1.5f));
				mSpriteBatch->Draw(mSelectionRectangleTexture, SimpleMath::Rectangle(x1, y2, abs(x2 - x1), 1.5f));

				mSpriteBatch->Draw(mSelectionRectangleTexture, SimpleMath::Rectangle(x1, y1, 1.5f, abs(y2 - y1)));
				mSpriteBatch->Draw(mSelectionRectangleTexture, SimpleMath::Rectangle(x2, y1, 1.5f, abs(y2 - y1)));
			}
		}
		

		mSpriteBatch->End();
		mRenderStateHelper->RestoreAll();

		HRESULT hr = mSwapChain->Present(0, 0);
		if (FAILED(hr))
		{
			throw GameException("IDXGISwapChain::Present() failed.", hr);
		}
	}

	void RenderingGame::PaintMode1(){
		
		paintMode1flag = false;
		
		cutSceneId = 5;
		mapLevel = true;

		for (int i = 0; i < mGameManager->GetListOfUnits().size(); i++) {
					if (mGameManager->GetListOfUnits().at(i)->GetUnitID() == 100 ) {
						FarbaMan* farbaM = mGameManager->GetListOfUnits().at(mGameManager->GetListOfUnits().size()-1)->As<FarbaMan>();
							if (farbaM->GetinPaintArea()) {
								//mGameManager->SelectingGroundsFake(580, -200);
								std::vector<XMFLOAT2> tmp;
								tmp.push_back(XMFLOAT2(590.0f, -200.0f));
								farbaM->StartMoving(tmp);
								farbaM->StartPainting();
								tmp.clear();
							}
					}
		}
	}

	void RenderingGame::ClearMode1() {
		for (int i = 0; i < mGameManager->GetListOfUnits().size(); i++) {
			GreenSoldier* sold = mGameManager->GetListOfUnits().at(i)->As<GreenSoldier>();
			if (sold->getIsSelected()) {
				sold->SetToClean();
			}
		}
	}
}