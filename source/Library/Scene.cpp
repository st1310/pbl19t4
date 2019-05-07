#include "Scene.h"

namespace Library{

Scene::Scene(int sceneId, std::string fileName)
	:mFileName(fileName), SceneId(sceneId)
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
	this->GameObjects.clear();
}

void Scene::Serialize()
{
	std::ofstream file(mRepositoryPath + mFilePath + mFileName);

	if (file.good() == true)
	{	
		for (int i = 0; i < GameObjects.size(); i++)
		{
			std::vector<std::string> lines = GameObjects.at(i)->Serialize();

			// Fix EOF in file
			for (int j = 0; j < lines.size(); j++)
			{

				if(i == (GameObjects.size() - 1) && j == (lines.size() - 1))
					file << lines.at(j);

				else
					file << lines.at(j) << "\n";
			}			
		}
		
		file.close();
	}
}

std::vector<SerializableGameObject> Scene::LoadFromFile()
{
	std::vector<SerializableGameObject> deserializableGameObjects = std::vector<SerializableGameObject>();

	std::fstream file;
	file.open(mRepositoryPath + mFilePath + mFileName, std::ios::in | std::ios::out);

	if (file.good() == true)
	{		
		while (!file.eof()) {

			std::vector<std::string> deserialisabledValues = std::vector<std::string>();

			// 1 - name 2-4 position 5-7 rotation 8-10 scale
			for (int i = 0; i < 10; i++)
			{
				std::string value;
				getline(file, value);
				deserialisabledValues.push_back(value);
			}

			SerializableGameObject serializableGameObject = SerializableGameObject(deserialisabledValues);
			deserializableGameObjects.push_back(serializableGameObject);
		}

		file.close();		
	}

	return deserializableGameObjects;
}

}