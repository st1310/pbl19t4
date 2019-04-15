#pragma once

#include "Camera.h"
#include "Colliders.h"

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

		const Colliders* GetColliders() const;
		void SetCollider(Colliders* collider);

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

		std::vector<Colliders*> collidableWalls;
		Colliders* mCollider;
		KeyboardComponent* mKeyboard;
		MouseComponent* mMouse;

	private:
		FirstPersonCamera(const FirstPersonCamera& rhs);
		FirstPersonCamera& operator=(const FirstPersonCamera& rhs);
	};
}
