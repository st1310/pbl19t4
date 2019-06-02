#pragma once

#include "DrawableGameComponent.h"
#include "KeyboardComponent.h"
#include "SerializableGameObject.h"
#include "RenderStateHelper.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "CollisionNode.h"
#include "Game.h"
#include <iomanip> 
#include <fstream>
#include <sstream>

namespace Library
{
	class Scene : public DrawableGameComponent
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

		void rewriteListOfNodes(std::vector<CollisionNode*>& list);
		std::vector<CollisionNode*> getListOfNode();

		void ClearUnitList();
		void AddUnitToList(GameComponent* unit);
		void RewriteUnitList(std::vector<GameComponent*> newListOfUnits);
		void RemoveUnitFromList(GameComponent* unit);
		std::vector<GameComponent*> GetUnitList();

	protected:
		KeyboardComponent* mKeyboard;
		RenderStateHelper mRenderStateHelper;
		SpriteBatch* mSpriteBatch;
		SpriteFont* mSpriteFont;
		XMFLOAT2 mTextPosition;

		std::vector<GameComponent*> listOfUnits;

	private:
		std::string mFileName;
		std::vector<CollisionNode*> listOfNodes;
	};
}

