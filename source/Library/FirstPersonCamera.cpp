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
		mMouseSensitivity(DefaultMouseSensitivity), mRotationRate(DefaultRotationRate), mMovementRate(DefaultMovementRate), mCollider(nullptr),
		mNode(nullptr)
	{
	}

	FirstPersonCamera::FirstPersonCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance)
		: Camera(game, fieldOfView, aspectRatio, nearPlaneDistance, farPlaneDistance), mKeyboard(nullptr), mMouse(nullptr),
		mMouseSensitivity(DefaultMouseSensitivity), mRotationRate(DefaultRotationRate), mMovementRate(DefaultMovementRate), mCollider(),
		mNode(nullptr)

	{
	}

	FirstPersonCamera::~FirstPersonCamera()
	{
		mKeyboard = nullptr;
		mMouse = nullptr;
		mCollider = nullptr;
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

	BoundingFrustum* FirstPersonCamera::GetFrustrum()
	{
		return mCollider;
	}

	void FirstPersonCamera::SetFrustrum(BoundingFrustum* collider)
	{
		if (collider != nullptr)
			mCollider = collider;
		else
			mCollider = new BoundingFrustum(ViewProjectionMatrix());
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
	}

	void FirstPersonCamera::Update(const GameTime& gameTime)
	{
		bool moved = false;
		XMFLOAT2 movementAmount = Vector2Helper::Zero;
		if (mKeyboard != nullptr)
		{
			if (mKeyboard->IsKeyDown(DIK_W) || mKeyboard->IsKeyDown(DIK_UPARROW))
			{
				movementAmount.y = 1.0f;
				moved = true;
			}

			if (mKeyboard->IsKeyDown(DIK_S) || mKeyboard->IsKeyDown(DIK_DOWNARROW))
			{
				movementAmount.y = -1.0f;
				moved = true;
			}

			if (mKeyboard->IsKeyDown(DIK_A) || mKeyboard->IsKeyDown(DIK_LEFTARROW))
			{
				movementAmount.x = -1.0f;
				moved = true;
			}

			if (mKeyboard->IsKeyDown(DIK_D) || mKeyboard->IsKeyDown(DIK_RIGHTARROW))
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

		//mCollider->Transform(mCollider, );

  		XMStoreFloat3(&mPosition, position);
		Camera::Update(gameTime);

		if (moved || mCollider == nullptr)
		{
				XMFLOAT4 dr;
				XMStoreFloat4(&dr, DirectionVector());
				if (dr.z >= 0)
					mCollider = new BoundingFrustum(mPosition, dr, mPosition.x + 10.f, mPosition.x - 10.f,
						mPosition.y + 10.5f, mPosition.y - 10.5f, mPosition.z + mNearPlaneDistance, mPosition.z + mFarPlaneDistance);
				else mCollider = new BoundingFrustum(mPosition, dr, mPosition.x + 10.f, mPosition.x - 10.f,
					mPosition.y + 10.5f, mPosition.y - 10.5f, -(mPosition.z - mNearPlaneDistance), mPosition.z - mFarPlaneDistance);

			mGame->SetNodesInFructum(NodeList::CheckNodesInsideCamera(mCollider, mGame->NodeList()));
		}

	}
}
