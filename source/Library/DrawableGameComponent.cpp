#include "DrawableGameComponent.h"

namespace Library
{
	RTTI_DEFINITIONS(DrawableGameComponent)

	DrawableGameComponent::DrawableGameComponent()
	: GameComponent(), mVisible(true), mCamera(nullptr), mRemoval(false)
	{
	}

	DrawableGameComponent::DrawableGameComponent(Game& game)
		: GameComponent(game), mVisible(true), mCamera(nullptr), mRemoval(false)
	{
	}

	DrawableGameComponent::DrawableGameComponent(Game& game, Camera& camera)
		: GameComponent(game), mVisible(true), mCamera(&camera), mRemoval(false)
	{
	}

	DrawableGameComponent::~DrawableGameComponent()
	{
	}

	bool DrawableGameComponent::Visible() const
	{
		return mVisible;
	}

	bool DrawableGameComponent::RemovalComponent() const
	{
		return mRemoval;
	}

	void DrawableGameComponent::SetVisible(bool visible)
	{
		mVisible = visible;
	}

	void DrawableGameComponent::SetRemovalComponent(bool removal)
	{
		mRemoval = removal;
	}

	Camera * DrawableGameComponent::GetCamera()
	{
		return mCamera;
	}

	void DrawableGameComponent::SetCamera(Camera * camera)
	{
		mCamera = camera;
	}

	void DrawableGameComponent::Draw(const GameTime & gameTime)
	{
	}
}