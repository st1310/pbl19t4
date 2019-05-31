#pragma once
#include "Common.h"
#include "Button.h"
#include "DrawableGameComponent.h"
#include "RenderStateHelper.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"

namespace Library 
{
	namespace Rendering
	{
		class GUI : public DrawableGameComponent {
		public:
			GUI();
			~GUI();

			virtual void Initialize() override;
			virtual void Update(const GameTime& gameTime) override;
			virtual void Draw(const GameTime& gameTime) override;


			void AddButton(Button* button);
			std::vector<Button*> GetButtonList();

		private:
			std::vector<Button*> buttonsList;
			SpriteBatch* mSpriteBatch;
			SpriteFont* mSpriteFont;

		};
	}
}

