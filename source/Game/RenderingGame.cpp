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

namespace Rendering
{
	const XMVECTORF32 RenderingGame::BackgroundColor = ColorHelper::CornflowerBlue;
	
	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring & windowClass, const std::wstring & windowTitle, int showCommand)
		: Game(instance, windowClass, windowTitle, showCommand),
		mFpsComponent(nullptr), mSkybox(nullptr),
		mRenderStateHelper(nullptr),
		mDirectInput(nullptr), mKeyboard(nullptr), mMouse(nullptr), mCamera(nullptr),
		mSpriteBatch(nullptr), mSpriteFont(nullptr), mMouseTextPosition(0.0f, 20.0f),
		mGameManager(nullptr), buttonClicked(false), unitActiveFlag(false), mUnitGuiTexture(nullptr),
		mUnitGuiTextureBlack(nullptr)
	{
		mDepthStencilBufferEnabled = true;
		mMultiSamplingEnabled = true;
	}

	RenderingGame::~RenderingGame()
	{
	}

	void RenderingGame::Initialize()
	{
		if (FAILED(DirectInput8Create(mInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&mDirectInput, nullptr)))
		{
			throw GameException("DirectInput8Create() failed");
		}

		mKeyboard = new KeyboardComponent(*this, mDirectInput);
		mComponents.push_back(mKeyboard);
		mServices.AddService(KeyboardComponent::TypeIdClass(), mKeyboard);

		mMouse = new MouseComponent(*this, mDirectInput);
		mComponents.push_back(mMouse);
		mServices.AddService(MouseComponent::TypeIdClass(), mMouse);

		mCollC = new Colliders();
		mCamera = new GameCamera(*this);
		CollisionNode* newNode = new CollisionNode({-5.f, -20.f, 0.f}, { 5.f, 20.f, 50.f });

		//For debug
		mComponents.push_back(mCamera);
		mServices.AddService(GameCamera::TypeIdClass(), mCamera);
		//mServices.AddService(FirstPersonCamera::TypeIdClass(), mCamera);

		// For game
		//mComponents.push_back(mGameCamera);
		//mServices.AddService(GameCamera::TypeIdClass(), mGameCamera);

//=======
		mSkybox = new SkyboxComponent(*this, *mCamera, L"Content\\Textures\\Maskonaive2_1024.dds", 100.0f);
		mComponents.push_back(mSkybox);
		mServices.AddService(SkyboxComponent::TypeIdClass(), mSkybox);

		mGameManager = new GameManager(*this, *mCamera);
		mComponents.push_back(mGameManager);

		mFpsComponent = new FpsComponent(*this); // Components using SpriteBach should perform Draw last
		mComponents.push_back(mFpsComponent);
		//mFpsComponent->SetAlwaysDrawn(true);

		mRenderStateHelper = new RenderStateHelper(*this);

		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		mSpriteBatch = new SpriteBatch(mDirect3DDeviceContext);
		mSpriteFont = new SpriteFont(mDirect3DDevice, L"Content\\Fonts\\Arial_14_Regular.spritefont");

		std::wostringstream textureName;
		textureName << L"content\\Textures\\pepeColor.png";

		std::wostringstream textureName1;
		textureName1 << L"content\\Textures\\pepeBlack.png";

		HRESULT hr = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), textureName.str().c_str(), nullptr, &mUnitGuiTexture);
		HRESULT hr1 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), textureName1.str().c_str(), nullptr, &mUnitGuiTextureBlack);

		if (FAILED(hr))
			throw GameException("CreateWICTextureFromFile() failed.", hr);

		Game::Initialize();


		//mCamera->SetPosition(0.0f, 10.0f, 20.0f);		
		mCamera->SetPosition(0.0f, 70.0f, 20.0f);

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

		if (mKeyboard->WasKeyPressedThisFrame(DIK_1))
			mGameManager->StartScene(MENU_LEVEL);

		if (mKeyboard->WasKeyPressedThisFrame(DIK_2))
			mGameManager->StartScene(DAY_LEVEL);

		if (mKeyboard->WasKeyPressedThisFrame(DIK_3))
			mGameManager->StartScene(TRAIN_LEVEL);

		if (mKeyboard->WasKeyPressedThisFrame(DIK_4))
			mGameManager->StartScene(CITY_LEVEL);

		if (mKeyboard->WasKeyPressedThisFrame(DIK_5))
			mGameManager->StartScene(CREATION_KIT_LEVEL);
		if(mKeyboard->WasKeyPressedThisFrame(DIK_6))
			mGameManager->StartScene(PATHFINDER_TEST);

		if (mMouse->WasButtonPressedThisFrame(MouseButtonsLeft))
		{
			if (mMouse->Y() > 640.0f && mMouse->X() > 20.0f  && mMouse->X() < 369.0f) {

			}
			else {
				if (mKeyboard->IsKeyHeldDown(DIKEYBOARD_LCONTROL)) {
					mGameManager->SelectingUnits(mMouse->X(), mMouse->Y(), true);
				}

				else {
					mGameManager->SelectingUnits(mMouse->X(), mMouse->Y(), false);

				}
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

		std::wostringstream mouseLabel;
		mouseLabel << L"Mouse Position: " << mMouse->X() << ", "
			<< mMouse->Y() << " Mouse Wheel: " << mMouse->Wheel();
		mSpriteFont->DrawString(mSpriteBatch, mouseLabel.str().c_str(), mMouseTextPosition);

		if (true) 
		{
			for (int i = 0; i < mGameManager->GetListOfUnits().size(); i++) 
			{
				if (mGameManager->GetListOfUnits().at(i)->GetUnitID()==mGameManager->unitID) 
					mSpriteBatch->Draw(mUnitGuiTexture, SimpleMath::Rectangle(0.0f + 50 * i, 630.0f, 150.0f, 150.0f));

				else 
					mSpriteBatch->Draw(mUnitGuiTextureBlack, SimpleMath::Rectangle(0 + 50 * i, 660.0f, 120.0f, 120.0f));			
			}
		}

		if (mGameManager->GetunitsReadyToMove()) {
			std::wostringstream tmp;
			tmp << "GroundActive";
			mSpriteFont->DrawString(mSpriteBatch, tmp.str().c_str(), XMFLOAT2(50.0f, 300.0f), Colors::Blue);
		}

		if (mMouse->Y() > 295.0f && mMouse->Y() < 307.0f && mMouse->X() > 104.0f  && mMouse->X() < 200.0f) 
		{
			std::wostringstream startGame;
			startGame << "Start Game";
			mSpriteFont->DrawString(mSpriteBatch, startGame.str().c_str(), XMFLOAT2(100.0f, 300.0f), Colors::Blue);
		}
		else {
			std::wostringstream startGame;
			startGame << "Start Game";
			mSpriteFont->DrawString(mSpriteBatch, startGame.str().c_str(), XMFLOAT2(100.0f, 300.0f), Colors::White);
		}
		
		if (mMouse->Y() > 313.0f && mMouse->Y() < 327.0f && mMouse->X() > 104.0f  && mMouse->X() < 200.0f) {
			std::wostringstream endGame;
			endGame << "Quit Game";
			mSpriteFont->DrawString(mSpriteBatch, endGame.str().c_str(), XMFLOAT2(100.0f, 320.0f), Colors::Blue);
		}
		else {
			std::wostringstream endGame;
			endGame << "Quit Game";
			mSpriteFont->DrawString(mSpriteBatch, endGame.str().c_str(), XMFLOAT2(100.0f, 320.0f), Colors::White);
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