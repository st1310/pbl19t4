#pragma once

#include "Camera.h"
#include "Colliders.h"
#include "CollisionNode.h"

namespace Library
{
	class KeyboardComponent;
	class MouseComponent;

	class GameCamera :
		public Camera
	{
		RTTI_DECLARATIONS(GameCamera, Camera)

	public:
		GameCamera(Game& game);
		GameCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance);

		virtual ~GameCamera();

		const KeyboardComponent& GetKeyboard() const;
		void SetKeyboard(KeyboardComponent& keyboard);

		const MouseComponent& GetMouse() const;
		void SetMouse(MouseComponent& mouse);


		void SetFrustrum(BoundingFrustum* collider);
		BoundingFrustum* GetFrustrum();
		void SetCollisionNode(CollisionNode* node);

		void SendColliderList(std::vector<CollisionNode*> newNodeList);
		void SetMinimalAndMaximalPoint(XMFLOAT2 minimalPoint, XMFLOAT2 maximalPoint);

		float& MouseSensitivity();
		float& RotationRate();
		float& MovementRate();

		void SceneLoad();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;

		static const float DefaultMouseSensitivity;
		static const float DefaultRotationRate;
		static const float DefaultMovementRate;

	protected:
		float mMouseSensitivity;
		float mRotationRate;
		float mMovementRate;

		CollisionNode* mNode;
		KeyboardComponent* mKeyboard;
		MouseComponent* mMouse;

	private:
		bool firstTime;
		GameCamera(const GameCamera& rhs);
		GameCamera& operator=(const GameCamera& rhs);



		XMFLOAT2 mMinimalPointAtMap = XMFLOAT2(-100,-1200); // First value must be -100
		XMFLOAT2 mMaximalPointAtMap = XMFLOAT2(1200, 1200);

		const float mMoveCameraFactor = 2; //0.12;
		const float mStartYPosition = 70;
		float mCurrentYPosition = mStartYPosition;
		const float mMinYPosition = 30;
		const float mMaxYPosition = 150;
		const float mYMoveCameraFactor = 5;

		long mLastWheelPosition = 0;
	};
}
