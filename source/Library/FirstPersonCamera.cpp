#include "FirstPersonCamera.h"
#include "Game.h"
#include "GameTime.h"
#include "KeyboardComponent.h"
#include "MouseComponent.h"
#include "VectorHelper.h"

namespace Library
{
	RTTI_DEFINITIONS(FirstPersonCamera)

	const float FirstPersonCamera::DefaultRotationRate = XMConvertToRadians(1.0f);
	const float FirstPersonCamera::DefaultMovementRate = 10.0f;
	const float FirstPersonCamera::DefaultMouseSensitivity = 100.0f;

	FirstPersonCamera::FirstPersonCamera(Game& game)
		: Camera(game), mKeyboard(nullptr), mMouse(nullptr),
		mMouseSensitivity(DefaultMouseSensitivity), mRotationRate(DefaultRotationRate), mMovementRate(DefaultMovementRate), mCollider(),
		collidableWalls()
	{
		collidableWalls = game.ColliderList();
	}

	FirstPersonCamera::FirstPersonCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance)
		: Camera(game, fieldOfView, aspectRatio, nearPlaneDistance, farPlaneDistance), mKeyboard(nullptr), mMouse(nullptr),
		mMouseSensitivity(DefaultMouseSensitivity), mRotationRate(DefaultRotationRate), mMovementRate(DefaultMovementRate), mCollider(),
		collidableWalls()

	{
		collidableWalls = game.ColliderList();
	}

	FirstPersonCamera::~FirstPersonCamera()
	{
		mKeyboard = nullptr;
		mMouse = nullptr;
		mCollider = nullptr;
		collidableWalls.clear();
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

	const Colliders* FirstPersonCamera::GetColliders() const
	{
		return mCollider;
	}

	void FirstPersonCamera::SetCollider(Colliders* collider)
	{
		mCollider = collider;
		if (mCollider->IsEmpty())
		{
			mCollider->PushNewBoundingBox(new BoundingBox({ mPosition.x - 0.1f, mPosition.y + 0.1f, mPosition.z - 0.1f }, { mPosition.x + 0.1f, mPosition.y - 0.1f, mPosition.z + 0.1f }));
		}
	}

	void FirstPersonCamera::SendColliderList(std::vector<Colliders*> collList)
	{
		collidableWalls = collList;
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
		XMFLOAT2 movementAmount = Vector2Helper::Zero;
		if (mKeyboard != nullptr)
		{
			if (mKeyboard->IsKeyDown(DIK_W) || mKeyboard->IsKeyDown(DIK_UPARROW))
			{
				movementAmount.y = 1.0f;
			}

			if (mKeyboard->IsKeyDown(DIK_S) || mKeyboard->IsKeyDown(DIK_DOWNARROW))
			{
				movementAmount.y = -1.0f;
			}

			if (mKeyboard->IsKeyDown(DIK_A) || mKeyboard->IsKeyDown(DIK_LEFTARROW))
			{
				movementAmount.x = -1.0f;
			}

			if (mKeyboard->IsKeyDown(DIK_D) || mKeyboard->IsKeyDown(DIK_RIGHTARROW))
			{
				movementAmount.x = 1.0f;
			}
		}

		XMFLOAT2 rotationAmount = Vector2Helper::Zero;
		if ((mMouse != nullptr) && (mMouse->IsButtonHeldDown(MouseButtonsLeft)))
		{
			LPDIMOUSESTATE mouseState = mMouse->CurrentState();
			rotationAmount.x = -mouseState->lX * mMouseSensitivity;
			rotationAmount.y = -mouseState->lY * mMouseSensitivity;
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

		mCollider->Move(position);
		if(!mCollider->CheckCollision(collidableWalls))
			XMStoreFloat3(&mPosition, position);
		else mCollider->Move(XMLoadFloat3(&mPosition));

		Camera::Update(gameTime);
	}
}
