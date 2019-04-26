#pragma once

#include "GameComponent.h"
#include "CollisionNode.h"

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

		void SetAlwaysDrawn(bool ShouldBe);
		void SetNode(CollisionNode* newNode);
		CollisionNode* GetNode();
		bool IsThisMustBeAlwaysDrawn();

		Camera* GetCamera();
		void SetCamera(Camera* camera);

		virtual void Draw(const GameTime& gameTime);

	protected:
		bool mShouldBeAlwaysDrawn;
		CollisionNode* mNode;
		bool mVisible;
		Camera* mCamera;

	private:
		DrawableGameComponent(const DrawableGameComponent& rhs);
		DrawableGameComponent& operator=(const DrawableGameComponent& rhs);
	};
}