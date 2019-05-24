#pragma once

#include "Camera.h"
#include "Colliders.h"
#include "CollisionNode.h"

namespace Library
{
	class KeyboardComponent;
	class MouseComponent;

	class FirstPersonCamera :
		public Camera
	{
		RTTI_DECLARATIONS(FirstPersonCamera, Camera)

	public:
		FirstPersonCamera(Game& game);
		FirstPersonCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance);

		virtual ~FirstPersonCamera();

		const KeyboardComponent& GetKeyboard() const;
		void SetKeyboard(KeyboardComponent& keyboard);

		const MouseComponent& GetMouse() const;
		void SetMouse(MouseComponent& mouse);

		
		void SetFrustrum(BoundingFrustum* collider);
		BoundingFrustum* GetFrustrum();
		void SetCollisionNode(CollisionNode* node);

		void SendColliderList(std::vector<CollisionNode*> newNodeList);

		float& MouseSensitivity();
		float& RotationRate();
		float& MovementRate();

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
		bool cameraMode;
		bool firstTime;
		FirstPersonCamera(const FirstPersonCamera& rhs);
		FirstPersonCamera& operator=(const FirstPersonCamera& rhs);
	};
}
