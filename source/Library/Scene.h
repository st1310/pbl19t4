#pragma once

#include <iostream>
#include <list>
#include "DrawableGameComponent.h"
namespace Library
{

	class Scene
	{
	public:
		Scene(int sceneId);
		~Scene();
		int SceneId;
		std::list<DrawableGameComponent> GameObjects;
	};
}

