#pragma once

#include "GameObject.h"
#include "RenderStateHelper.h"
#include "CollisionNode.h"

using namespace Library;

namespace Library
{
	class KeyboardComponent;
	class Effect;
	class Model;
	class TextureMappingMaterial;
	class SkinnedModelMaterial;
	class AnimationPlayer;
	class MultipleLightsMaterial;
}

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

namespace Rendering
{
	class StaticGameObject : public GameObject
	{
		RTTI_DECLARATIONS(StaticGameObject, GameObject)

	public:
		StaticGameObject(Game& game, Camera& camera, const char *className, 
			XMFLOAT3 startPosition, XMFLOAT3 startRotation, XMFLOAT3 startScale, bool needCollision = false);
		~StaticGameObject();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		virtual void BuildBoundingBox(XMFLOAT3 radius) override;

		void UpdateColorFilterMaterial();
		void UpdateGenericColorFilter(const GameTime& gameTime);
	private:
		StaticGameObject();
		StaticGameObject(const StaticGameObject& rhs);
		StaticGameObject& operator=(const StaticGameObject& rhs);

		void UpdateOptions();

		MultipleLightsMaterial* mMaterial;

		bool needsCollision;
	};
}
