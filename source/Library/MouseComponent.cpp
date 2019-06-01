#include "MouseComponent.h"
#include "Game.h"
#include "GameTime.h"
#include "GameException.h"

namespace Library
{
	RTTI_DEFINITIONS(MouseComponent)

	MouseComponent::MouseComponent(Game& game, LPDIRECTINPUT8 directInput) :
		GameComponent(game), mDirectInput(directInput), mDevice(nullptr),
		mX(0), mY(0), mWheel(0)
	{
		assert(mDirectInput != nullptr);
		ZeroMemory(&mCurrentState, sizeof(mCurrentState));
		ZeroMemory(&mLastState, sizeof(mLastState));

		POINT p;
		GetCursorPos(&p);
		ScreenToClient(mGame->WindowHandle(), &p);
		mX = p.x;
		mY = p.y;
	}

	MouseComponent::~MouseComponent()
	{
		if (mDevice != nullptr)
		{
			mDevice->Unacquire();
			mDevice->Release();
			mDevice = nullptr;
		}
	}

	LPDIMOUSESTATE MouseComponent::CurrentState()
	{
		return &mCurrentState;
	}

	LPDIMOUSESTATE MouseComponent::LastState()
	{
		return &mLastState;
	}

	void MouseComponent::Initialize()
	{
		if (FAILED(mDirectInput->CreateDevice(GUID_SysMouse, &mDevice, nullptr)))
		{
			throw GameException("IDIRECTINPUT8::CreateDevice() failed");
		}

		if (FAILED(mDevice->SetDataFormat(&c_dfDIMouse)))
		{
			throw GameException("IDIRECTINPUTDEVICE8::SetDataFormat() failed");
		}

		if (FAILED(mDevice->SetCooperativeLevel(mGame->WindowHandle(),
			DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		{
			throw GameException("IDIRECTINPUTDEVICE8::SetCooperativeLevel() failed");
		}

		if (FAILED(mDevice->Acquire()))
		{
			throw GameException("IDIRECTINPUTDEVICE8::Acquire() failed");
		}
	}

	void MouseComponent::Update(const GameTime & gameTime)
	{
		if (mDevice != nullptr)
		{
			memcpy(&mLastState, &mCurrentState, sizeof(mCurrentState));

			if (FAILED(mDevice->GetDeviceState(sizeof(mCurrentState),
				&mCurrentState)))
			{
				// Try to reacquire the device
				// TODO: add cerr or warning if the device can't be reacquired
				if (SUCCEEDED(mDevice->Acquire()))
				{
					if (FAILED(mDevice->GetDeviceState(sizeof(mCurrentState), &mCurrentState)))
					{
						return;
					}
				}
			}

			// Accumulate positions
			// TODO: check for mX < 0, mY < 0 and for max mX, mY
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(mGame->WindowHandle(), &p);
			mX = p.x;
			mY = p.y;

			mWheel += mCurrentState.lZ;
		}
	}

	long MouseComponent::X() const
	{
		return mX;
	}

	long MouseComponent::Y() const
	{
		return mY;
	}

	long MouseComponent::Wheel() const
	{
		return mWheel;
	}

	bool MouseComponent::IsButtonUp(MouseButtons button) const
	{
		return ((mCurrentState.rgbButtons[button] & 0x80) == 0);
	}

	bool MouseComponent::IsButtonDown(MouseButtons button) const
	{
		return ((mCurrentState.rgbButtons[button] & 0x80) != 0);
	}

	bool MouseComponent::WasButtonUp(MouseButtons button) const
	{
		return ((mLastState.rgbButtons[button] & 0x80) == 0);
	}

	bool MouseComponent::WasButtonDown(MouseButtons button) const
	{
		return ((mLastState.rgbButtons[button] & 0x80) != 0);
	}

	bool MouseComponent::WasButtonPressedThisFrame(MouseButtons button) const
	{
		return (IsButtonDown(button) && WasButtonUp(button));
	}

	bool MouseComponent::WasButtonReleasedThisFrame(MouseButtons button) const
	{
		return (IsButtonUp(button) && WasButtonDown(button));
	}

	bool MouseComponent::IsButtonHeldDown(MouseButtons button) const
	{
		return (IsButtonDown(button) && WasButtonDown(button));
	}

}
