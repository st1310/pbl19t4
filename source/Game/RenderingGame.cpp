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
		mUnitGuiTextureBlack(nullptr), mSelectionRectangleTexture(nullptr)
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

		mComponents.push_back(mCamera);
		mServices.AddService(GameCamera::TypeIdClass(), mCamera);

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

		std::wostringstream textureForRactangle;
		textureForRactangle << L"content\\Textures\\whiteRect75.png";


		HRESULT hr = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), textureName.str().c_str(), nullptr, &mUnitGuiTexture);
		HRESULT hr1 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), textureName1.str().c_str(), nullptr, &mUnitGuiTextureBlack);
		HRESULT hr2 = DirectX::CreateWICTextureFromFile(this->Direct3DDevice(), this->Direct3DDeviceContext(), textureForRactangle.str().c_str(), nullptr, &mSelectionRectangleTexture);

		if (FAILED(hr))
			throw GameException("CreateWICTextureFromFile() failed.", hr);

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
			if (gameTime.TotalGameTime() - timeFromPressed.TotalGameTime() >= 0.3)
			{
				mouse2Pos.x = 0.0f + mMouse->X();
				mouse2Pos.y = 0.0f + mMouse->Y();
				selectedOnce = true;
			}
		}
		else if (mMouse->WasButtonReleasedThisFrame(MouseButtonsLeft))
		{
			if (selectedOnce)
			{
				mGameManager->SelectingUnits(mouse1Pos.x, mouse1Pos.y, mouse2Pos.x, mouse2Pos.y);
				selectedOnce = false;
			}
			else  mGameManager->SelectingUnits(mouse1Pos.x, mouse1Pos.y);
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
					mSpriteBatch->Draw(mUnitGuiTexture, SimpleMath::Rectangle(0.f + 50 * i, 630.0f, 150.0f, 150.0f));

				else 
					mSpriteBatch->Draw(mUnitGuiTextureBlack, SimpleMath::Rectangle(0.f + 50 * i, 660.0f, 120.0f, 120.0f));			
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