#include "Scene.h"
#include "Utility.h"

namespace Library{
	RTTI_DEFINITIONS(Scene)

Scene::Scene(Game& game, Camera& camera, int sceneId, std::string fileName) :
	DrawableGameComponent(game, camera), mFileName(fileName), SceneId(sceneId), mKeyboard(nullptr),
		mRenderStateHelper(game), mSpriteBatch(nullptr), mSpriteFont(nullptr), mTextPosition(0.0f, 400.0f)
{

}

Scene::~Scene()
{
}

void Scene::Start(Game& game, Camera& camera)
{
}

void Scene::Update(const GameTime& gameTime)
{
}

void Scene::Initialize()
{
	SetCurrentDirectory(Utility::ExecutableDirectory().c_str());
	mKeyboard = (KeyboardComponent*)mGame->Services().GetService(KeyboardComponent::TypeIdClass());
	assert(mKeyboard != nullptr);

	mSpriteBatch = new SpriteBatch(mGame->Direct3DDeviceContext());
	mSpriteFont = new SpriteFont(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");
}

void Scene::Clear()
{
	this->GameObjects.clear();
}

void Scene::SaveToFile()
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