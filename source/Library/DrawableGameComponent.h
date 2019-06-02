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

		Camera* GetCamera();
		void SetCamera(Camera* camera);

		virtual void Draw(const GameTime& gameTime);
		virtual CollisionNode* getNode();

		void SetUnitID(int id);
		int GetUnitID();
		void SetComponentSelected(bool value);
		bool GetComponentSelected();

	protected:
		CollisionNode* mNode;
		bool mVisible;
		Camera* mCamera;

	private:
		DrawableGameComponent(const DrawableGameComponent& rhs);
		DrawableGameComponent& operator=(const DrawableGameComponent& rhs);
		int unitID;
		bool componentSelected = false;
	};
}