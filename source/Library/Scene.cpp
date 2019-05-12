#include "Scene.h"
#include "Utility.h"

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
	std::string  a = Utility::CurrentDirectory();
	SetCurrentDirectory(Utility::LibraryDirectory().c_str());
	std::string  b = Utility::CurrentDirectory();
	std::ofstream file(mFileName);

	if (file.good() == true)
	{	
		for (int i = 0; i < GameObjects.size(); i++)
		{			
			SerializableGameObject serializabledGameObject = SerializableGameObject();
			std::vector<std::string> validLines = serializabledGameObject.Serialize(GameObjects.at(i));

			for (int j = 0; j < validLines.size(); j++)
			{
				if(i == (GameObjects.size() - 1) && j == (validLines.size() - 1))
					file << validLines.at(j);

				else
					file << validLines.at(j) << "\n";
			}			
		}
		
		file.close();
	}
}

std::vector<SerializableGameObject> Scene::LoadFromFile()
{
	std::vector<SerializableGameObject> deserializableGameObjects = std::vector<SerializableGameObject>();
	std::string  a = Utility::CurrentDirectory();
	SetCurrentDirectory(Utility::LibraryDirectory().c_str());
	std::string  b = Utility::CurrentDirectory();
	std::fstream file;
	file.open( mFileName, std::ios::in | std::ios::out);

	if (file.good() == true)
	{		
		int deserializabledLinesCount =  SerializableGameObject().DeserializedLinesCount;

		while (!file.eof()) 
		{
			std::vector<std::string> deserialisabledValues = std::vector<std::string>();

			for (int i = 0; i < deserializabledLinesCount; i++)
			{
				std::string value;
				getline(file, value);
				deserialisabledValues.push_back(value);
			}

			SerializableGameObject serializableGameObject = SerializableGameObject();
			serializableGameObject.Deserialize(deserialisabledValues);

			deserializableGameObjects.push_back(serializableGameObject);
		}

		file.close();		
	}

	return deserializableGameObjects;
}

}