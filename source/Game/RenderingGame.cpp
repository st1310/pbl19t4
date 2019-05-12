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
#include "TexturedModelMaterialDemo.h"
#include "GameManager.h"
#include "AnimatedGameObject.h"
#include "Utility.h"
#include "NodeList.h"

namespace Rendering
{
	const XMVECTORF32 RenderingGame::BackgroundColor = ColorHelper::CornflowerBlue;
	
	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring & windowClass, const std::wstring & windowTitle, int showCommand)
		: Game(instance, windowClass, windowTitle, showCommand),
		mFpsComponent(nullptr), mSkybox(nullptr),
		mRenderStateHelper(nullptr),
		mDirectInput(nullptr), mKeyboard(nullptr), mMouse(nullptr), mCamera(nullptr),
		mSpriteBatch(nullptr), mSpriteFont(nullptr), mMouseTextPosition(0.0f, 20.0f),
		mGameManager(nullptr), buttonClicked(false)
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
		mCamera = new FirstPersonCamera(*this);
		CollisionNode* newNode = new CollisionNode({-5.f, -20.f, 0.f}, { 5.f, 20.f, 50.f });

		mComponents.push_back(mCamera);
		mServices.AddService(FirstPersonCamera::TypeIdClass(), mCamera);


		BoundingBox* mDemoBox = new BoundingBox();
		mCollTM = new Colliders(mDemoBox);
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

		Game::Initialize();


		mCamera->SetPosition(0.0f, 0.0f, 20.0f);

		CollisionNode* additionalCheckNode = new CollisionNode({ -1000.f, -200.f, 0.f }, { 900.f, 190.f, 20.f });
		newNode->AddDynamicCollider(mCollC);
		newNode->AddStaticCollider(mCollTM);
		additionalCheckNode->AddNewChild(newNode);
		mNode.push_back(additionalCheckNode);
		

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

		if (mKeyboard->WasKeyPressedThisFrame(DIK_M))
		{
			mGameManager->Scenes[mGameManager->currentScene]->Serialize();
		}


		if (mKeyboard->WasKeyPressedThisFrame(DIK_N))
		{
			Bench* b = new Bench(*this, *mCamera);
			b->mIsEdited = true;
			mGameManager->Scenes[mGameManager->currentScene]->GameObjects.push_back(b);
			b->Initialize();

			//mComponents.push_back(b);
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_C))
		{
			mGameManager->StartScene(CREATION_KIT_LEVEL);
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_X))
		{
			mGameManager->StartScene(CITY_LEVEL);
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_1))
		{
			if (!buttonClicked) 
			{
				buttonClicked = true;
				for (GameComponent* component : mComponents) {
					DrawableGameComponent* drawableGameComponent = component->As<DrawableGameComponent>();
					if (drawableGameComponent != nullptr) {
						drawableGameComponent->SetVisible(false);
					}
				}
			}
			else
			{
				buttonClicked = false;
				for (GameComponent* component : mComponents) {
					DrawableGameComponent* drawableGameComponent = component->As<DrawableGameComponent>();
					if (drawableGameComponent != nullptr) {
						drawableGameComponent->SetVisible(true);
					}
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

		
		mRenderStateHelper->SaveAll();
		mSpriteBatch->Begin();

		std::wostringstream mouseLabel;
		mouseLabel << L"Mouse Position: " << mMouse->X() << ", "
			<< mMouse->Y() << " Mouse Wheel: " << mMouse->Wheel();
		mSpriteFont->DrawString(mSpriteBatch, mouseLabel.str().c_str(), mMouseTextPosition);

		if (mMouse->Y() > 295.0f && mMouse->Y() < 307.0f && mMouse->X() > 104.0f  && mMouse->X() < 200.0f) {
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