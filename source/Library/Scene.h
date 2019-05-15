#pragma once

#include "DrawableGameComponent.h"
#include "SerializableGameObject.h"
#include "CollisionNode.h"
#include <iomanip> 
#include <fstream>

namespace Library
{

	

	class Scene
	{
	public:
		Scene(int sceneId, std::string filePath);
		~Scene();		
		virtual void Start(Game& game, Camera& camera);
		void Clear();

		int SceneId;
		std::vector<GameComponent*> GameObjects;
		void Serialize();
		std::vector<SerializableGameObject> LoadFromFile();

		void rewriteListOfNodes(std::vector<CollisionNode*>& list);
		std::vector<CollisionNode*> getListOfNode();

		void ClearUnitList();
		void AddUnitToList(GameComponent* unit);
		void RewriteUnitList(std::vector<GameComponent*> newListOfUnits);
		void RemoveUnitFromList(GameComponent* unit);
		std::vector<GameComponent*> GetUnitList();

	protected:
		std::vector<GameComponent*> listOfUnits;
	private:
		std::string mFileName;
		std::vector<CollisionNode*> listOfNodes;
	};
}

