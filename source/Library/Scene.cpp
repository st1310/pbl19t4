#include "Scene.h"

namespace Library{

Scene::Scene(int sceneId, std::string filePath)
	:mFilePath(filePath), SceneId(sceneId)
{

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

void Scene::Serialize()
{
	std::ofstream file(mFilePath);
	if (file.good() == true)
	{	
		for (int i = 0; i < GameObjects.size(); i++)
		{
			std::vector<std::string> lines = GameObjects.at(i)->Serialize();
			for (int j = 0; j < lines.size(); j++)
				file << lines.at(j) << "\n";
		}
		
		file.close();
	}
}

void Scene::LoadFromFile()
{
	std::fstream file;
	file.open(mFilePath, std::ios::in | std::ios::out);
	if (file.good() == true)
	{
		
		
		/*
		for (int i = 0; i < size; i++)
		{
			std::vector<std::string> lines = GameObjects.at(i)->Serialize();
			for (int j = 0; j < lines.size; j++)
				file << lines.at(j);
		}
		*/
		file.close();
	}
}

}