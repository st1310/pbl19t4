#include "Scene.h"

namespace Library{

Scene::Scene()
{

}

Scene::Scene(int sceneId)
{
	this->SceneId = sceneId;
}


Scene::~Scene()
{
}

void Scene::Start(Game& game, Camera& camera)
{

}

void Scene::Clear()
{
	// TODO fix -- Szymon
	//this->GameObjects.clear();
}

}