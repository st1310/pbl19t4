#pragma once

#include "DrawableGameComponent.h"
namespace Library
{

	class Scene
	{
	public:
		Scene();
		Scene(int sceneId);
		~Scene();		
		void Start(Game& game, Camera& camera);
		void Clear();

		int SceneId;
		int Size;
		std::vector<DrawableGameComponent*> GameObjects;

	private:
		
	};
}

