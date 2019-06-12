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
		mGameManager(nullptr), buttonClicked(false), unitActiveFlag(false), mUnitGuiTexture(nullptr),
		mUnitGuiTextureBlack(nullptr), mSelectionRectangleTexture(nullptr), mOptionActionsBanner(nullptr)
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

		mFpsComponent = new FpsComponent(*this); // Components using SpriteBach should perform Draw last
		mComponents.push_back(mFpsComponent);

		mRenderStateHelper = new RenderStateHelper(*this);

		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		mSpriteBatch = new SpriteBatch(mDirect3DDeviceContext);
		mSpriteFont = new SpriteFont(mDirect3DDevice, L"Content\\Fonts\\Arial_14_Regular.spritefont");


		std::wostringstream textureName;
		textureName << L"content\\Textures\\pepeColor.png";

		std::wostringstream textureName1;
		textureName1 << L"content\\Textures\\pepeBlack.png";

		std::wostringstream textureForRactangle;
		textureForRactangle << L"content\\Textures\\whiteRect75.png";

		std::wostringstream textureOptionActionsBanner;
		textureOptionActionsBanner << L"content\\Textures\\optionsFrame.png";

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
		

		HRESULT hr = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), textureName.str().c_str(), nullptr, &mUnitGuiTexture);
		HRESULT hr1 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), textureName1.str().c_str(), nullptr, &mUnitGuiTextureBlack);
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
		{
			Exit();
		}

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

		if (mKeyboard->WasKeyPressedThisFrame(DIK_1)) {
			if (mGameManager->GetListOfUnits().size() >= 1) {
				keybordButtonSelectUnit = 0;
			}
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_2)) {
			if (mGameManager->GetListOfUnits().size() >= 2) {
				keybordButtonSelectUnit = 1;
			}
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_3)) {
			if (mGameManager->GetListOfUnits().size() >= 3) {
				keybordButtonSelectUnit = 2;
			}
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_4)) {
			if (mGameManager->GetListOfUnits().size() >= 4) {
				keybordButtonSelectUnit = 3;
			}
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_5)) {
			if (mGameManager->GetListOfUnits().size() >= 5) {
				keybordButtonSelectUnit = 4;
			}
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_6)) {
			if (mGameManager->GetListOfUnits().size() >= 6) {
				keybordButtonSelectUnit = 5;
			}
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_7)) {
			if (mGameManager->GetListOfUnits().size() >= 7) {
				keybordButtonSelectUnit = 6;
			}
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_8)) {
			if (mGameManager->GetListOfUnits().size() >= 8) {
				keybordButtonSelectUnit = 7;
			}
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_9)) {
			if (mGameManager->GetListOfUnits().size() >= 9) {
				keybordButtonSelectUnit = 8;
			}
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_0)) {
			if (mGameManager->GetListOfUnits().size() >= 10) {
				keybordButtonSelectUnit = 9;
			}
		}



		float diffBetweenUnitsIcons = (250.0f / mGameManager->GetListOfUnits().size());

		
	
		if (showUnitsGui == false) {
			if (mMouse->X() > 320.0f  && mMouse->X() < 720.0f && mMouse->Y() > 680.0f) {
				showUnitsGui = true;
			}
		}
		else if (showUnitsGui == true) {
			if (mMouse->X() < 320.0f  || mMouse->X() > 720.0f || mMouse->Y() < 570.0f) {
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
				mGameManager->SelectingGrounds(mMouse->X(), mMouse->Y());
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
		
		mRenderStateHelper->SaveAll();
		mSpriteBatch->Begin();
		int posOffset = 0;
		std::wostringstream mouseLabel;
		mouseLabel << L"Mouse Position: " << mMouse->X() << ", "
			<< mMouse->Y() << " Mouse Wheel: " << mMouse->Wheel();
		mSpriteFont->DrawString(mSpriteBatch, mouseLabel.str().c_str(), mMouseTextPosition);

		if (showUnitsGui) {
			mSpriteBatch->Draw(mUnitsBanner, SimpleMath::Rectangle(320.0f, 570.0f, 400.0f, 220.0f));
			posOffset = 1;

		}
		else if (!showUnitsGui) 
		{
			mSpriteBatch->Draw(mUnitsBanner, SimpleMath::Rectangle(320.0f, 650.0f, 400.0f, 220.0f));
			posOffset = 0;
		}
		

		if (true)
		{
			for (int i = 0; i < mGameManager->GetListOfUnits().size(); i++)
			{

				if (mGameManager->GetListOfUnits().at(i)->GetUnitID() == mGameManager->unitID) 
					mSpriteBatch->Draw(mUnitGuiTexture, SimpleMath::Rectangle(360.f + 50 * i, 630.0f - posOffset * 80, 100.0f, 100.0f));					

				else
				{
					if (mMouse->X() > (360 + i * 60) && (mMouse->X() < (360 + i * 60 + 60) && mMouse->Y() > 680 ) || keybordButtonSelectUnit ==i)
					{
						mSpriteBatch->Draw(mUnitGuiTexture, SimpleMath::Rectangle(360.f + 50 * i, 630.0f - posOffset * 80, 100.0f, 100.0f));

						if (mMouse->WasButtonReleasedThisFrame(MouseButtonsLeft) || keybordButtonSelectUnit == i) {
							showUnitDetail = true;
							bool checkCopy = false;

							keybordButtonSelectUnit = -1;

							for (int x = 0; x < indexSelectedGuiButtons.size() ;x++) {	//sprawdzanie czy juz raz kliknieto na ikone, jesli tak to nie zapisuj do wektora indeksow zaznaczonych drugi raz
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
					}
					else
					{
						mSpriteBatch->Draw(mUnitGuiTextureBlack, SimpleMath::Rectangle(360.f + 50 * i, 700.0f - posOffset * 80, 80.0f, 80.0f));

					}
				}
			}
		}

		if (!(indexSelectedGuiButtons.empty())) {
			for (int i = 0; i < indexSelectedGuiButtons.size(); i++) {
				mSpriteBatch->Draw(mUnitGuiTexture, SimpleMath::Rectangle(360.f + 50 * indexSelectedGuiButtons.at(i), 630.0f - posOffset * 80, 100.0f, 100.0f));
			}
		}
		else {
			showUnitDetail = false;
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
			
			
			mSpriteBatch->Draw(mPortraitBanner, SimpleMath::Rectangle(765.0f, 330.0f, 250.0f, 250.0f), Colors::White);
			mSpriteBatch->Draw(mOptionActionsBanner, SimpleMath::Rectangle(720.0f, 560.0f, 300.0f, 250.0f));

			if (indexSelectedGuiButtons.size() > 1) {
				mSpriteBatch->Draw(mMultiSelectionPortrait, SimpleMath::Rectangle(800, 350, 200.0f, 200.0f));
				mSpriteBatch->Draw(mPaintIconNO, SimpleMath::Rectangle(750.0f, 600.0f, 70.0f, 70.0f));
				mSpriteBatch->Draw(mCleanIconNO, SimpleMath::Rectangle(830.0f, 597.0f, 80.0f, 80.0f));
				if (mMouse->X() > 925.0f && mMouse->X() < 995.0f  && mMouse->Y() > 605.0f  && mMouse->Y() < 665.0f) {
					mSpriteBatch->Draw(mPatrolIconMAYBE, SimpleMath::Rectangle(920.0f, 595.0f, 80.0f, 80.0f));
					if (mMouse->IsButtonHeldDown(MouseButtonsLeft)) {
						mSpriteBatch->Draw(mPatrolIconCLICKED, SimpleMath::Rectangle(920.0f, 595.0f, 80.0f, 80.0f));
					}
				}
					
				else
					mSpriteBatch->Draw(mPatrolIconYES, SimpleMath::Rectangle(920.0f, 595.0f, 80.0f, 80.0f));

				if (mMouse->X() > 750.0f && mMouse->X() < 815.0f  && mMouse->Y() > 680.0f  && mMouse->Y() < 745.0f) {
					mSpriteBatch->Draw(mHideIconMAYBE, SimpleMath::Rectangle(740.0f, 670.0f, 90.0f, 90.0f));
					if (mMouse->IsButtonHeldDown(MouseButtonsLeft)) {
						mSpriteBatch->Draw(mHideIconCLICKED, SimpleMath::Rectangle(740.0f, 670.0f, 90.0f, 90.0f));
					}
				}
					
				else
					mSpriteBatch->Draw(mHideIconYES, SimpleMath::Rectangle(740.0f, 670.0f, 90.0f, 90.0f));

				if (mMouse->X() > 840.0f && mMouse->X() < 905.0f  && mMouse->Y() > 680.0f  && mMouse->Y() < 745.0f)
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
					mSpriteBatch->Draw(mNoiseIconYES, SimpleMath::Rectangle(920.0f, 680.0f, 85.0f, 85.0f));
			}
			else {
				mSpriteBatch->Draw(mUnitGuiTexture, SimpleMath::Rectangle(800, 350, 200.0f, 200.0f));
				if (mMouse->X() > 750.0f && mMouse->X() < 815.0f  && mMouse->Y() > 605.0f  && mMouse->Y() < 665.0f) {
					mSpriteBatch->Draw(mPaintIconMAYBE, SimpleMath::Rectangle(750.0f, 600.0f, 70.0f, 70.0f));
					if (mMouse->IsButtonHeldDown(MouseButtonsLeft)) {
						mSpriteBatch->Draw(mPaintIconCLICKED, SimpleMath::Rectangle(750.0f, 600.0f, 70.0f, 70.0f));	
					}
					
				}
					
				else
					mSpriteBatch->Draw(mPaintIconYES, SimpleMath::Rectangle(750.0f, 600.0f, 70.0f, 70.0f));
				
				if (mMouse->X() > 840.0f && mMouse->X() < 905.0f  && mMouse->Y() > 605.0f  && mMouse->Y() < 665.0f) {
					mSpriteBatch->Draw(mCleanIconMAYBE, SimpleMath::Rectangle(830.0f, 597.0f, 80.0f, 80.0f));
					if (mMouse->IsButtonHeldDown(MouseButtonsLeft)) {
						mSpriteBatch->Draw(mCleanIconCLICKED, SimpleMath::Rectangle(830.0f, 597.0f, 80.0f, 80.0f));
					}
				}
					
				else
					mSpriteBatch->Draw(mCleanIconYES, SimpleMath::Rectangle(830.0f, 597.0f, 80.0f, 80.0f));
				
				if (mMouse->X() > 925.0f && mMouse->X() < 995.0f  && mMouse->Y() > 605.0f  && mMouse->Y() < 665.0f) {
					mSpriteBatch->Draw(mPatrolIconMAYBE, SimpleMath::Rectangle(920.0f, 595.0f, 80.0f, 80.0f));
					if (mMouse->IsButtonHeldDown(MouseButtonsLeft)) {
						mSpriteBatch->Draw(mPatrolIconCLICKED, SimpleMath::Rectangle(920.0f, 595.0f, 80.0f, 80.0f));
					}
				}
					
				else
					mSpriteBatch->Draw(mPatrolIconYES, SimpleMath::Rectangle(920.0f, 595.0f, 80.0f, 80.0f));
				
				if (mMouse->X() > 750.0f && mMouse->X() < 815.0f  && mMouse->Y() > 680.0f  && mMouse->Y() < 745.0f) {
					mSpriteBatch->Draw(mHideIconMAYBE, SimpleMath::Rectangle(740.0f, 670.0f, 90.0f, 90.0f));
					if (mMouse->IsButtonHeldDown(MouseButtonsLeft)) {
						mSpriteBatch->Draw(mHideIconCLICKED, SimpleMath::Rectangle(740.0f, 670.0f, 90.0f, 90.0f));
					}
				}
					
				else
					mSpriteBatch->Draw(mHideIconYES, SimpleMath::Rectangle(740.0f, 670.0f, 90.0f, 90.0f));

				if (mMouse->X() > 840.0f && mMouse->X() < 905.0f  && mMouse->Y() > 680.0f  && mMouse->Y() < 745.0f) {
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
					mSpriteBatch->Draw(mNoiseIconYES, SimpleMath::Rectangle(920.0f, 680.0f, 85.0f, 85.0f));
			}
			
		}/*mStopIconMAYBE*/
			
		
		

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

			mSpriteBatch->Draw(mSelectionRectangleTexture, SimpleMath::Rectangle(x1, y1, abs(x2-x1), 1.5f));
			mSpriteBatch->Draw(mSelectionRectangleTexture, SimpleMath::Rectangle(x1, y2, abs(x2 - x1), 1.5f));

			mSpriteBatch->Draw(mSelectionRectangleTexture, SimpleMath::Rectangle(x1, y1, 1.5f, abs(y2 - y1)));
			mSpriteBatch->Draw(mSelectionRectangleTexture, SimpleMath::Rectangle(x2, y1, 1.5f, abs(y2 - y1)));
		}

		mSpriteBatch->End();
		mRenderStateHelper->RestoreAll();

		HRESULT hr = mSwapChain->Present(0, 0);
		if (FAILED(hr))
		{
			throw GameException("IDXGISwapChain::Present() failed.", hr);
		}
	}
}