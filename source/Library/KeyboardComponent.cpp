#include "KeyboardComponent.h"
#include "Game.h"
#include "GameTime.h"
#include "GameException.h"

namespace Library
{
	RTTI_DEFINITIONS(KeyboardComponent)

	KeyboardComponent::KeyboardComponent(Game& game, LPDIRECTINPUT8 directInput) :
		GameComponent(game), mDirectInput(directInput), mDevice(nullptr)
	{
		assert(mDirectInput != nullptr);
		ZeroMemory(mCurrentState, sizeof(mCurrentState));
		ZeroMemory(mLastState, sizeof(mLastState));
	}

	KeyboardComponent::~KeyboardComponent()
	{
		if (mDevice != nullptr)
		{
			mDevice->Unacquire();
			mDevice->Release();
			mDevice = nullptr;
		}
	}

	const byte * const KeyboardComponent::CurrentState() const
	{
		return mCurrentState;
	}

	const byte * const KeyboardComponent::LastState() const
	{
		return mLastState;
	}

	void KeyboardComponent::Initialize()
	{
		if (FAILED(mDirectInput->CreateDevice(GUID_SysKeyboard, &mDevice, nullptr)))
		{
			throw GameException("IDIRECTINPUT8::CreateDevice() failed");
		}

		if (FAILED(mDevice->SetDataFormat(&c_dfDIKeyboard)))
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

	void KeyboardComponent::Update(const GameTime & gameTime)
	{
		if (mDevice != nullptr)
		{
			memcpy(mLastState, mCurrentState, sizeof(mCurrentState));

			if (FAILED(mDevice->GetDeviceState(sizeof(mCurrentState), (LPVOID)mCurrentState)))
			{
				// Try to reacquire the device
				// TODO: add cerr or warning if the device can't be reacquired
				if (SUCCEEDED(mDevice->Acquire()))
				{
					mDevice->GetDeviceState(sizeof(mCurrentState), (LPVOID)mCurrentState);
				}
			}
		}
	}

	bool KeyboardComponent::IsKeyUp(byte key) const
	{
		return ((mCurrentState[key] & 0x80) == 0);
	}

	bool KeyboardComponent::IsKeyDown(byte key) const
	{
		return ((mCurrentState[key] & 0x80) != 0);
	}

	bool KeyboardComponent::WasKeyUp(byte key) const
	{
		return ((mLastState[key] & 0x80) == 0);
	}

	bool KeyboardComponent::WasKeyDown(byte key) const
	{
		return ((mLastState[key] & 0x80) != 0);
	}

	bool KeyboardComponent::WasKeyPressedThisFrame(byte key) const
	{
		return (IsKeyDown(key) && WasKeyUp(key));
	}

	bool KeyboardComponent::WasKeyReleasedThisFrame(byte key) const
	{
		return (IsKeyUp(key) && WasKeyDown(key));
	}

	bool KeyboardComponent::IsKeyHeldDown(byte key) const
	{
		return (IsKeyDown(key) && WasKeyDown(key));
	}

}
