#include "RenderingGame.h"
#include <sstream>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "GameException.h"
#include "KeyboardComponent.h"
#include "MouseComponent.h"
#include "FpsComponent.h"
#include "FirstPersonCamera.h"
#include "SkyboxComponent.h"
#include "ColorHelper.h"
#include "RenderStateHelper.h"
#include "TexturedModelDemo.h"
#include "TexturedModelMaterialDemo.h"
#include "Utility.h"

namespace Rendering
{
	const XMVECTORF32 RenderingGame::BackgroundColor = ColorHelper::CornflowerBlue;
	
	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring & windowClass, const std::wstring & windowTitle, int showCommand)
		: Game(instance, windowClass, windowTitle, showCommand),
		mFpsComponent(nullptr), mSkybox(nullptr),
		mDirectInput(nullptr), mKeyboard(nullptr), mMouse(nullptr),
		mSpriteBatch(nullptr), mSpriteFont(nullptr), mMouseTextPosition(0.0f, 20.0f),
		mTMDemo(nullptr), mTMMDemo(nullptr)
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

		mCamera = new FirstPersonCamera(*this);
		mComponents.push_back(mCamera);
		mServices.AddService(FirstPersonCamera::TypeIdClass(), mCamera);

		mSkybox = new SkyboxComponent(*this, *mCamera, L"Content\\Textures\\Maskonaive2_1024.dds", 100.0f);
		mComponents.push_back(mSkybox);
		mServices.AddService(SkyboxComponent::TypeIdClass(), mSkybox);

		/*mTMDemo = new TexturedModelDemo(*this, *mCamera);
		mComponents.push_back(mTMDemo);*/

		mTMMDemo = new TexturedModelMaterialDemo(*this, *mCamera, L"Content\\Textures\\checker.dds");
		mComponents.push_back(mTMMDemo);

		mFpsComponent = new FpsComponent(*this); // Components using SpriteBach should perform Draw last
		mComponents.push_back(mFpsComponent);

		mRenderStateHelper = new RenderStateHelper(*this);

		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		mSpriteBatch = new SpriteBatch(mDirect3DDeviceContext);
		mSpriteFont = new SpriteFont(mDirect3DDevice, L"Content\\Fonts\\Arial_14_Regular.spritefont");

		Game::Initialize();

		mCamera->SetPosition(0.0f, 0.0f, 20.0f);
	}

	void RenderingGame::Shutdown()
	{
		DeleteObject(mTMDemo);
		DeleteObject(mTMMDemo);
		DeleteObject(mKeyboard);
		DeleteObject(mMouse);
		DeleteObject(mFpsComponent);
		DeleteObject(mSkybox);
		DeleteObject(mSpriteBatch);
		DeleteObject(mSpriteFont);
		DeleteObject(mCamera);

		ReleaseObject(mDirectInput);

		Game::Shutdown();
	}

	void RenderingGame::Update(const GameTime & gameTime)
	{
		if (mKeyboard->WasKeyPressedThisFrame(DIK_ESCAPE))
		{
			Exit();
		}

		/*if (mKeyboard->WasKeyPressedThisFrame(DIK_F))
		{
			mMDemo->SetVisible(!mMDemo->Visible());
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_G))
		{
			mMDemo->SetEnabled(!mMDemo->Enabled());
		}*/

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

		mSpriteBatch->End();
		mRenderStateHelper->RestoreAll();

		HRESULT hr = mSwapChain->Present(0, 0);
		if (FAILED(hr))
		{
			throw GameException("IDXGISwapChain::Present() failed.", hr);
		}
	}
}