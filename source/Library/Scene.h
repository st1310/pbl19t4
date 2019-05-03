#pragma once

#include "DrawableGameComponent.h"
#include <fstream>
namespace Library
{

	class Scene
	{
	public:
		Scene(int sceneId, std::string filePath);
		~Scene();		
		void Start(Game& game, Camera& camera);
		void Clear();

		int SceneId;
		std::vector<GameComponent*> GameObjects;
		void Serialize();
		void LoadFromFile();

	private:
		std::string mFilePath;
	};
}

