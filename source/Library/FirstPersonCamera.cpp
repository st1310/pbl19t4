#include "FirstPersonCamera.h"
#include "Game.h"
#include "GameTime.h"
#include "KeyboardComponent.h"
#include "MouseComponent.h"
#include "VectorHelper.h"
#include "NodeList.h"

namespace Library
{
	RTTI_DEFINITIONS(FirstPersonCamera)

	const float FirstPersonCamera::DefaultRotationRate = XMConvertToRadians(1.0f);
	const float FirstPersonCamera::DefaultMovementRate = 10.0f;
	const float FirstPersonCamera::DefaultMouseSensitivity = 100.0f;

	FirstPersonCamera::FirstPersonCamera(Game& game)
		: Camera(game), mKeyboard(nullptr), mMouse(nullptr),
		mMouseSensitivity(DefaultMouseSensitivity), mRotationRate(DefaultRotationRate), mMovementRate(DefaultMovementRate), mNode(nullptr)
	{
	}

	FirstPersonCamera::FirstPersonCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance)
		: Camera(game, fieldOfView, aspectRatio, nearPlaneDistance, farPlaneDistance), mKeyboard(nullptr), mMouse(nullptr),
		mMouseSensitivity(DefaultMouseSensitivity), mRotationRate(DefaultRotationRate), mMovementRate(DefaultMovementRate),	mNode(nullptr)

	{
	}

	FirstPersonCamera::~FirstPersonCamera()
	{
		mKeyboard = nullptr;
		mMouse = nullptr;
		mNode = nullptr;
	}

	const KeyboardComponent& FirstPersonCamera::GetKeyboard() const
	{
		return *mKeyboard;
	}

	void FirstPersonCamera::SetKeyboard(KeyboardComponent& keyboard)
	{
		mKeyboard = &keyboard;
	}

	const MouseComponent& FirstPersonCamera::GetMouse() const
	{
		return *mMouse;
	}

	void FirstPersonCamera::SetMouse(MouseComponent& mouse)
	{
		mMouse = &mouse;
	}

	void FirstPersonCamera::SetCollisionNode(CollisionNode* node)
	{
		mNode = node;
	}


	float&FirstPersonCamera::MouseSensitivity()
	{
		return mMouseSensitivity;
	}


	float& FirstPersonCamera::RotationRate()
	{
		return mRotationRate;
	}

	float& FirstPersonCamera::MovementRate()
	{
		return mMovementRate;
	}

	void FirstPersonCamera::Initialize()
	{
		// TODO: assert that the objects are present in the service container
		mKeyboard = (KeyboardComponent*)mGame->Services().GetService(KeyboardComponent::TypeIdClass());
		mMouse = (MouseComponent*)mGame->Services().GetService(MouseComponent::TypeIdClass());

		Camera::Initialize();
		firstTime = false;
	}

	void FirstPersonCamera::Update(const GameTime& gameTime)
	{
		bool moved = false;
		XMFLOAT2 movementAmount = Vector2Helper::Zero;
		if (mKeyboard != nullptr)
		{
			if (mKeyboard->IsKeyDown(DIK_W))
			{
				movementAmount.y = 1.0f;
				moved = true;
			}

			if (mKeyboard->IsKeyDown(DIK_S))
			{
				movementAmount.y = -1.0f;
				moved = true;
			}

			if (mKeyboard->IsKeyDown(DIK_A))
			{
				movementAmount.x = -1.0f;
				moved = true;
			}

			if (mKeyboard->IsKeyDown(DIK_D))
			{
				movementAmount.x = 1.0f;
				moved = true;
			}
		}

		XMFLOAT2 rotationAmount = Vector2Helper::Zero;
		if ((mMouse != nullptr) && (mMouse->IsButtonHeldDown(MouseButtonsLeft)))
		{
			LPDIMOUSESTATE mouseState = mMouse->CurrentState();
			rotationAmount.x = -mouseState->lX * mMouseSensitivity;
			rotationAmount.y = -mouseState->lY * mMouseSensitivity;
			moved = true;
		}

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
				planes.push_back(XMVectorSet(1.0f, 0.f, -(mPosition.x + 30.f), 0.f));
				planes[0] = DirectX::Internal::XMPlaneTransform(planes[0], dirHelp, position);
				planes[0] = XMPlaneNormalize(planes[0]);

				//left slope
				planes.push_back(XMVectorSet(-1.0f, 0.f, (mPosition.x - 30.f), 0.f));
				planes[1] = DirectX::Internal::XMPlaneTransform(planes[1], dirHelp, position);
				planes[1] = XMPlaneNormalize(planes[1]);

				//upper slope
				planes.push_back(XMVectorSet(0.0f, 1.f, -(mPosition.y + 30.f), 0.f));
				planes[2] = DirectX::Internal::XMPlaneTransform(planes[2], dirHelp, position);
				planes[2] = XMPlaneNormalize(planes[2]);
				//lower slope
				planes.push_back(XMVectorSet(0.0f, -1.f, (mPosition.y - 30.f), 0.f));
				planes[3] = DirectX::Internal::XMPlaneTransform(planes[3], dirHelp, position);
				planes[3] = XMPlaneNormalize(planes[3]);

				//near and far slope
				if(mDirection.z > 0.f)
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
