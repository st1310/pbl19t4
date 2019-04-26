#include "DrawableGameComponent.h"

namespace Library
{
	RTTI_DEFINITIONS(DrawableGameComponent)

		DrawableGameComponent::DrawableGameComponent()
		: GameComponent(), mVisible(true), mCamera(nullptr), mShouldBeAlwaysDrawn(false)
	{
	}

	DrawableGameComponent::DrawableGameComponent(Game& game)
		: GameComponent(game), mVisible(true), mCamera(nullptr), mShouldBeAlwaysDrawn(false)
	{
	}

	DrawableGameComponent::DrawableGameComponent(Game& game, Camera& camera)
		: GameComponent(game), mVisible(true), mCamera(&camera), mShouldBeAlwaysDrawn(false)
	{
	}

	DrawableGameComponent::~DrawableGameComponent()
	{
	}

	bool DrawableGameComponent::Visible() const
	{
		return mVisible;
	}

	void DrawableGameComponent::SetVisible(bool visible)
	{
		mVisible = visible;
	}

	void DrawableGameComponent::SetAlwaysDrawn(bool ShouldBe)
	{
		mShouldBeAlwaysDrawn = ShouldBe;
	}

	void DrawableGameComponent::SetNode(CollisionNode* newNode)
	{
		mNode = newNode;
	}

	CollisionNode* DrawableGameComponent::GetNode()
	{
		return mNode;
	}

	bool DrawableGameComponent::IsThisMustBeAlwaysDrawn()
	{
		return mShouldBeAlwaysDrawn;
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