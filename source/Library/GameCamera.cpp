#include "GameCamera.h"
#include "Game.h"
#include "GameTime.h"
#include "KeyboardComponent.h"
#include "MouseComponent.h"
#include "VectorHelper.h"
#include "NodeList.h"

namespace Library
{
	RTTI_DEFINITIONS(GameCamera)

		const float GameCamera::DefaultRotationRate = XMConvertToRadians(1.0f);
	const float GameCamera::DefaultMovementRate = 10.0f;
	const float GameCamera::DefaultMouseSensitivity = 100.0f;

	GameCamera::GameCamera(Game& game)
		: Camera(game), mKeyboard(nullptr), mMouse(nullptr),
		mMouseSensitivity(DefaultMouseSensitivity), mRotationRate(DefaultRotationRate), mMovementRate(DefaultMovementRate), mNode(nullptr)

	{
	}

	GameCamera::GameCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance)
		: Camera(game, fieldOfView, aspectRatio, nearPlaneDistance, farPlaneDistance), mKeyboard(nullptr), mMouse(nullptr),
		mMouseSensitivity(DefaultMouseSensitivity), mRotationRate(DefaultRotationRate), mMovementRate(DefaultMovementRate), mNode(nullptr)

	{
	}

	GameCamera::~GameCamera()
	{
		mKeyboard = nullptr;
		mMouse = nullptr;
		mNode = nullptr;
	}

	const KeyboardComponent& GameCamera::GetKeyboard() const
	{
		return *mKeyboard;
	}

	void GameCamera::SetKeyboard(KeyboardComponent& keyboard)
	{
		mKeyboard = &keyboard;
	}

	const MouseComponent& GameCamera::GetMouse() const
	{
		return *mMouse;
	}

	void GameCamera::SetMouse(MouseComponent& mouse)
	{
		mMouse = &mouse;
	}

	void GameCamera::SetCollisionNode(CollisionNode* node)
	{
		mNode = node;
	}


	float&GameCamera::MouseSensitivity()
	{
		return mMouseSensitivity;
	}


	float& GameCamera::RotationRate()
	{
		return mRotationRate;
	}

	float& GameCamera::MovementRate()
	{
		return mMovementRate;
	}

	void GameCamera::Initialize()
	{
		// TODO: assert that the objects are present in the service container
		mKeyboard = (KeyboardComponent*)mGame->Services().GetService(KeyboardComponent::TypeIdClass());
		mMouse = (MouseComponent*)mGame->Services().GetService(MouseComponent::TypeIdClass());

		Camera::Initialize();
		firstTime = false;

		XMVECTOR rotationVector = XMLoadFloat2(new XMFLOAT2(-50,-50)) * mRotationRate;
		XMVECTOR right = XMLoadFloat3(&mRight);

		XMMATRIX pitchMatrix = XMMatrixRotationAxis(right, XMVectorGetY(rotationVector));
		XMMATRIX yawMatrix = XMMatrixRotationX(XMVectorGetX(rotationVector));
		ApplyRotation(XMMatrixMultiply(pitchMatrix, yawMatrix));
		UpdateViewMatrix();

		mLastWheelPosition = mMouse->Wheel();
	}

	void GameCamera::SceneLoad()
	{
		firstTime = false;
	}

	void GameCamera::Update(const GameTime& gameTime)
	{
		bool moved = false;
		XMFLOAT2 movementAmount = Vector2Helper::Zero;
		if (mKeyboard->IsKeyDown(DIK_W))
		{
			XMFLOAT3 cameraPositon = Position();
			cameraPositon.z -= mMoveCameraFactor;
			SetPosition(cameraPositon);
			moved = true;
		}
		if (mKeyboard->IsKeyDown(DIK_S))
		{
			XMFLOAT3 cameraPositon = Position();
			cameraPositon.z += mMoveCameraFactor;
			SetPosition(cameraPositon);
			moved = true;
		}
		if (mKeyboard->IsKeyDown(DIK_A))
		{
			XMFLOAT3 cameraPositon = Position();
			cameraPositon.x -= mMoveCameraFactor;
			SetPosition(cameraPositon);
			moved = true;
		}
		if (mKeyboard->IsKeyDown(DIK_D))
		{
			XMFLOAT3 cameraPositon = Position();
			cameraPositon.x += mMoveCameraFactor;
			SetPosition(cameraPositon);
			moved = true;
		}
		if (mMouse->WasButtonPressedThisFrame(MouseButtonsMiddle))
		{
			// reset Y Position
			XMFLOAT3 cameraPositon = Position();
			mCurrentYPosition = mStartYPosition;
			cameraPositon.y = mCurrentYPosition;
			SetPosition(cameraPositon);
			moved = true;
		}
		if (mMouse->Wheel() > mLastWheelPosition)
		{
			XMFLOAT3 cameraPositon = Position();

			if (mCurrentYPosition > mMinYPosition)
			{
				mCurrentYPosition -= mYMoveCameraFactor;
				cameraPositon.y = mCurrentYPosition;
				SetPosition(cameraPositon);		
				moved = true;
			}

			mLastWheelPosition = mMouse->Wheel();
		}

		if (mMouse->Wheel() < mLastWheelPosition)
		{
			XMFLOAT3 cameraPositon = Position();

			if (mCurrentYPosition < mMaxYPosition)
			{
				mCurrentYPosition += mYMoveCameraFactor;
				cameraPositon.y = mCurrentYPosition;
				SetPosition(cameraPositon);
				moved = true;
			}

			mLastWheelPosition = mMouse->Wheel();
		}

		XMFLOAT2 rotationAmount = Vector2Helper::Zero;
		/*
		if ((mMouse != nullptr) && (mMouse->IsButtonHeldDown(MouseButtonsLeft) && cameraMode))
		{
			LPDIMOUSESTATE mouseState = mMouse->CurrentState();
			rotationAmount.x = -mouseState->lX * mMouseSensitivity;
			rotationAmount.y = -mouseState->lY * mMouseSensitivity;
			moved = true;
		}*/

		float elapsedTime = (float)gameTime.ElapsedGameTime();
		XMVECTOR rotationVector = XMLoadFloat2(&rotationAmount) * mRotationRate * elapsedTime;
		XMVECTOR right = XMLoadFloat3(&mRight);

		XMMATRIX pitchMatrix = XMMatrixRotationAxis(right, XMVectorGetY(rotationVector));
		XMMATRIX yawMatrix = XMMatrixRotationY(XMVectorGetX(rotationVector));

		ApplyRotation(XMMatrixMultiply(pitchMatrix, yawMatrix));

		XMVECTOR position = XMLoadFloat3(&mPosition);
		XMVECTOR movement = XMLoadFloat2(&movementAmount) * mMovementRate * elapsedTime;

		XMVECTOR strafe = right * XMVectorGetX(movement);
		position += strafe;

		XMVECTOR forward = XMLoadFloat3(&mDirection) * XMVectorGetY(movement);
		position += forward;

		XMFLOAT3 checkPos;
		XMStoreFloat3(&checkPos, position);

		XMStoreFloat3(&mPosition, position);
		Camera::Update(gameTime);

		

		if (moved || !firstTime)
		{
			float extender = 50.f + mCurrentYPosition * 1.5f;

			// 0 - right; 1 -left; 2 - up; 3 - down; 4 - near; 5 - far 
			std::vector<XMVECTOR> planes;
			XMVECTOR dirHelp = XMVECTOR();

			//Try to "smooth" border values
			dirHelp.m128_f32[0] = mDirection.x > 0.f ? (mDirection.x < 0.08f ? 0.08f : mDirection.x < 0.984f ? mDirection.x : 0.984f)
				: (mDirection.x > -0.08f ? -0.08f : mDirection.x > -0.9f ? mDirection.x : -0.9f);

			dirHelp.m128_f32[1] = mDirection.y > 0.f ? (mDirection.y < 0.08f ? 0.08f : mDirection.y < 0.984f ? mDirection.y : 0.984f)
				: (mDirection.y > -0.08f ? -0.08f : mDirection.y > -0.984f ? mDirection.y : -0.984f);

			dirHelp.m128_f32[2] = mDirection.z > 0.f ? (mDirection.z < 0.08f ? 0.08f : mDirection.z < 0.984f ? mDirection.z : 0.984f)
				: (mDirection.z > -0.08f ? -0.08f : mDirection.z > -0.984f ? mDirection.z : -0.984f);

			//right slope
			planes.push_back(XMVectorSet(1.0f, 0.f, -(mPosition.x + extender), 0.f));
			planes[0] = DirectX::Internal::XMPlaneTransform(planes[0], dirHelp, position);
			planes[0] = XMPlaneNormalize(planes[0]);

			//left slope
			planes.push_back(XMVectorSet(-1.0f, 0.f, (mPosition.x - extender), 0.f));
			planes[1] = DirectX::Internal::XMPlaneTransform(planes[1], dirHelp, position);
			planes[1] = XMPlaneNormalize(planes[1]);

			//upper slope
			planes.push_back(XMVectorSet(0.0f, 1.f, -(mPosition.y + extender), 0.f));
			planes[2] = DirectX::Internal::XMPlaneTransform(planes[2], dirHelp, position);
			planes[2] = XMPlaneNormalize(planes[2]);
			//lower slope
			planes.push_back(XMVectorSet(0.0f, -1.f, (mPosition.y - extender), 0.f));
			planes[3] = DirectX::Internal::XMPlaneTransform(planes[3], dirHelp, position);
			planes[3] = XMPlaneNormalize(planes[3]);

			//near and far slope
			if (mDirection.z > 0.f)
			{
				planes.push_back(XMVectorSet(0.0f, 0.f, -1.0f, (mPosition.z - 1.f)));
				planes.push_back(XMVectorSet(0.0f, 0.f, 1.0f, -(mPosition.z + FarPlaneDistance())));
			}
			else
			{
				planes.push_back(XMVectorSet(0.0f, 0.f, 1.0f, -(mPosition.z + 1.f)));
				planes.push_back(XMVectorSet(0.0f, 0.f, -1.0f, (mPosition.z - FarPlaneDistance())));
			}

			planes[4] = DirectX::Internal::XMPlaneTransform(planes[4], dirHelp, position);
			planes[4] = XMPlaneNormalize(planes[4]);

			planes[5] = DirectX::Internal::XMPlaneTransform(planes[5], dirHelp, position);
			planes[5] = XMPlaneNormalize(planes[5]);

			firstTime = true;
			mGame->SetNodesInFructum(NodeList::CheckNodesInsideCamera(planes, mGame->NodeList()));
		}

	}
}
