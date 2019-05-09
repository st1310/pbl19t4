#pragma once

#include "DrawableGameComponent.h"
#include "SerializableGameObject.h"
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

	private:
		std::string mFileName;
		std::string mFilePath = "Content\\Serializations\\";
		std::string mRepositoryPath = "C:\\Users\\kedzi\\OneDrive\\Desktop\\studia\\6\\PBL\\silnik\\pbl19t4\\";
	};
}

