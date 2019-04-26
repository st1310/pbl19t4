#pragma once

#include "GameComponent.h"
namespace Library
{
	class Camera;

	class DrawableGameComponent : public GameComponent
	{
		RTTI_DECLARATIONS(DrawableGameComponent, GameComponent)

	public:
		DrawableGameComponent();
		DrawableGameComponent(Game& game);
		DrawableGameComponent(Game& game, Camera& camera);
		virtual ~DrawableGameComponent();

		bool Visible() const;
		void SetVisible(bool visible);

		bool RemovalComponent() const;
		void SetRemovalComponent(bool visible);

		Camera* GetCamera();
		void SetCamera(Camera* camera);

		virtual void Draw(const GameTime& gameTime);

	protected:
		bool mVisible;
		bool mRemoval;
		Camera* mCamera;

	private:
		DrawableGameComponent(const DrawableGameComponent& rhs);
		DrawableGameComponent& operator=(const DrawableGameComponent& rhs);
	};
}