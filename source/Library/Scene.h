#pragma once

#include "DrawableGameComponent.h"
#include "KeyboardComponent.h"
#include "SerializableGameObject.h"
#include "RenderStateHelper.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "Game.h"
#include <iomanip> 
#include <fstream>
#include <sstream>

namespace Library
{
	class Scene :
		public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Scene, DrawableGameComponent)

	public:
		Scene(Game& game, Camera& camera, int sceneId, std::string filePath);
		~Scene();		

		virtual void Start(Game& game, Camera& camera);
		virtual void Update(const GameTime& gameTime);
		void Initialize();
		void Clear();

		int SceneId;
		std::vector<GameComponent*> GameObjects;
		void SaveToFile();
		std::vector<SerializableGameObject> LoadFromFile();

	protected:
		KeyboardComponent* mKeyboard;
		RenderStateHelper mRenderStateHelper;
		SpriteBatch* mSpriteBatch;
		SpriteFont* mSpriteFont;
		XMFLOAT2 mTextPosition;

	private:
		std::string mFileName;
	};
}

