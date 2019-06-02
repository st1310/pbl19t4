#include "Scene.h"
#include "Utility.h"
#include "SpotLight.h"
#include "ProxyModel.h"
#include "DirectionalLight.h"
#include "PointLight.h"

namespace Library
{
	RTTI_DEFINITIONS(Scene)

	Scene::Scene(Game& game, Camera& camera, int sceneId, std::string fileName) :
		DrawableGameComponent(game, camera), mFileName(fileName), SceneId(sceneId), mKeyboard(nullptr),
		mRenderStateHelper(game), mSpriteBatch(nullptr), mSpriteFont(nullptr), mTextPosition(0.0f, 400.0f), listOfNodes(), listOfUnits(), trigerrableObjects()
	{
		mDirectLights = std::vector<DirectionalLight*>();
		mPointLights = std::vector<PointLight*>();
		mSpotLights = std::vector<SpotLight*>();
	}

	Scene::~Scene()
	{
		listOfNodes.clear();
		listOfUnits.clear();
		trigerrableObjects.clear();
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
		SetCurrentDirectory(Utility::LibraryDirectory().c_str());
		std::ofstream file(mFileName);

		if (file.good() == true)
		{
			for (int i = 0; i < GameObjects.size(); i++)
			{
				SerializableGameObject serializabledGameObject = SerializableGameObject();
				std::vector<std::string> validLines = serializabledGameObject.Serialize(GameObjects.at(i));

				for (int j = 0; j < validLines.size(); j++)
				{
					if (i == (GameObjects.size() - 1) && j == (validLines.size() - 1))
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
		SetCurrentDirectory(Utility::LibraryDirectory().c_str());
		std::fstream file;
		file.open(mFileName, std::ios::in | std::ios::out);

		if (file.good() == true)
		{
			int deserializabledLinesCount = SerializableGameObject().DeserializedLinesCount;

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

	void Scene::rewriteListOfNodes(std::vector<CollisionNode*>& list)
	{
		listOfNodes = list;
	}

	std::vector<CollisionNode*> Scene::getListOfNode()
	{
		return listOfNodes;
	}

	void Scene::ClearUnitList()
	{
		listOfUnits.clear();
	}

	void Scene::AddUnitToList(DrawableGameComponent* unit)
	{
		listOfUnits.push_back(unit);
	}

	void Scene::RewriteUnitList(std::vector<DrawableGameComponent*> newListOfUnits)
	{
		listOfUnits = newListOfUnits;
	}

	void Scene::RemoveUnitFromList(DrawableGameComponent* unit)
	{
		auto itr = std::find(listOfUnits.begin(), listOfUnits.end(), unit);
		if (itr != listOfUnits.end())
			listOfUnits.erase(itr);
	}

	std::vector<DrawableGameComponent*> Scene::GetUnitList()
	{
		return listOfUnits;
	}

	void Scene::AddTriggerableObjectToList(DrawableGameComponent* object)
	{
		trigerrableObjects.push_back(object);
	}

	void Scene::RemoveTriggerableObjectFromList(DrawableGameComponent* object)
	{
		auto itr = std::find(trigerrableObjects.begin(), trigerrableObjects.end(), object);
		if (itr != trigerrableObjects.end())
			trigerrableObjects.erase(itr);
	}

	std::vector<DrawableGameComponent*> Scene::GetTriggerableObjects()
	{
		return trigerrableObjects;
	}

	Colliders* Scene::GetGroundCollider() {
		return groundCollider;
	}

	void Scene::SetGroudndCollider(Colliders* collider) {
		this->groundCollider = collider;
	}
	
	//Recursion - for dividing scene for smaller parts
	void Scene::BuildNodesRec(CollisionNode* parNode)
	{
		XMFLOAT3 posA = parNode->GetPosA();
		XMFLOAT3 posC = parNode->GetPosC();
		float partX = (posA.x - posC.x) * 0.5f;
		float partZ = (posC.z - posA.z) * 0.5f;

		if (partX < 100.f && partZ < 100.f)
			return;

		if (partX < 100.f) partX = 100.f;
		if (partZ < 100.f) partZ = 100.f;

		float toPntX = posA.x - partX;
		float toPntZ = posC.z - partZ;

		for (float prX = posA.x; prX >= posC.x; prX -= partX)
		{
			if (toPntX < posC.x) toPntX = posC.x;

			for (float prZ = posC.z; prZ >= posA.z; prZ -= partZ)
			{
				if (toPntZ < posA.z) toPntZ = posA.z;

				CollisionNode* chlColl = new CollisionNode({ prX, posA.y, toPntZ }, { toPntX, posC.y, prZ });
				chlColl->SetParent(parNode);
				parNode->AddNewChild(chlColl);
				BuildNodesRec(chlColl);
				toPntZ -= partZ;
			}

			toPntX -= partX;
		}
	}


	//Start building scene nodes from here
	//posA : max X, min Y and min Z
	//posC : min X, max Y and max Z
	void Scene::BuildNodesStart(XMFLOAT3 posA, XMFLOAT3 posC)
	{
		CollisionNode* mainColl = new CollisionNode(posA, posC);
		listOfNodes.push_back(mainColl);

		BuildNodesRec(mainColl);
			
	}

	std::vector<DirectionalLight*> Scene::GetDirectionalLights()
	{
		return mDirectLights;
	}

	std::vector<PointLight*> Scene::GetPointLights()
	{
		return mPointLights;
	}

	std::vector<SpotLight*> Scene::GetSpotLights()
	{
		return mSpotLights;
	}
}