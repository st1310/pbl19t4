#include "DayLevel.h"
#include "Camera.h"
#include "PointLight.h"

namespace Rendering
{
	DayLevel::DayLevel(Game& game, Camera& camera)
		:Scene(game, camera,DAY_LEVEL, "Content\\Serializations\\day_level.data")
	{
	}

	DayLevel::~DayLevel()
	{
	}

	void DayLevel::Start(Game& game, Camera& camera)
	{
		Initialize();
		InitializeLights();
		camera.SetPosition(0.0f, 70.0f, 20.0f);

		GameObjectPusher pusher = GameObjectPusher();
		pusher.listNode = this->getListOfNode();
		std::vector<GameComponent*> gameObjects = pusher.CreateAssets(game, camera, LoadFromFile());

		RewriteUnitList(pusher.listOfSoldiers);
		trigerrableObjects = pusher.triggerableObjects;

		for (int i = 0; i < gameObjects.size(); i++)
			this->GameObjects.push_back(gameObjects.at(i));

		// Add point Lights to soldiers
		for (int i = 0; i < gameObjects.size(); i++)
		{
			GameObject* gameObject = GameObjects.at(i)->As<GameObject>();
			std::string className = gameObject->GetName();

			if (className == "Soldier")
			{
				// Point Lights
				mPointLights.push_back(new PointLight(*mGame));
				int lastIndex = mPointLights.size() - 1;

				mPointLights.at(lastIndex)->SetRadius(30.0f);
				mPointLights.at(lastIndex)->SetPosition(gameObject->getPosition().x, gameObject->getPosition().y + 5, gameObject->getPosition().z);
				mPointLights.at(lastIndex)->SetColor(Colors::Red - SimpleMath::Vector3(0.0f, 0.0f, 0.5f));
			}
		}

		// Add spot Lights to lamps
		for (int i = 0; i < gameObjects.size(); i++)
		{
			GameObject* gameObject = GameObjects.at(i)->As<GameObject>();
			std::string className = gameObject->GetName();

			if (className == "SingleStreetLampPost")
			{
				// Point Lights
				mSpotLights.push_back(new SpotLight(*mGame));
				int lastIndex = mSpotLights.size() - 1;

				mSpotLights.at(lastIndex)->SetPosition(gameObject->getPosition().x, gameObject->getPosition().y + 5, gameObject->getPosition().z);
				mSpotLights.at(lastIndex)->SetRadius(30.0f);
				mSpotLights.at(lastIndex)->SetColor(Colors::Purple - SimpleMath::Vector3(0.0f, 0.0f, 0.2f));
				mSpotLights.at(lastIndex)->ApplyRotation(XMMatrixRotationX(XMConvertToRadians(-90.0f)));
			}
		}

		for (int i = 0; i < gameObjects.size(); i++)
		{
			GameObject* gameObject = GameObjects.at(i)->As<GameObject>();
			gameObject->Initialize();
			gameObject->SetLightsReferences(GetDirectionalLights(), GetPointLights(), GetSpotLights());
		}

		for (DrawableGameComponent* drwGm : trigerrableObjects)
		{
			GameObject* gmObj = drwGm->As<GameObject>();
			this->getListOfNode().at(0)->AddTriggerCollider(gmObj->getCollider());
		}
	}
}