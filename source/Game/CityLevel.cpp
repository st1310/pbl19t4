#include "CityLevel.h"
#include "Camera.h"
#include "PointLight.h"

namespace Rendering
{
	CityLevel::CityLevel(Game& game, Camera& camera)
		:Scene(game, camera, CITY_LEVEL, "Content\\Serializations\\city_level.data")
	{
	}

	CityLevel::~CityLevel()
	{
	}

	void CityLevel::Start(Game& game, Camera& camera)
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

		for (int i = 0; i < gameObjects.size(); i++)
		{
			GameObject* gameObject = GameObjects.at(i)->As<GameObject>();
			std::string className = gameObject->GetName();

			if (className == "Soldier")
			{
				GreenSoldier* soldier = gameObjects.at(i)->As<GreenSoldier>();
				mPointLights.push_back(soldier->GetPointLight());
			}

			gameObject->Initialize();
			gameObject->SetLightsReferences(GetDirectionalLights(), GetPointLights(), GetSpotLights());
		}

		for (DrawableGameComponent* drawableGameComponent : trigerrableObjects)
		{
			GameObject* gameObject = drawableGameComponent->As<GameObject>();
			this->getListOfNode().at(0)->AddTriggerCollider(gameObject->getCollider());
		}

		// Add point Lights to soldiers
		/*for (int i = 0; i < gameObjects.size(); i++)
		{
			GameObject* gameObject = gameObjects.at(i)->As<GameObject>();
			
		}*/

		// Add spot Lights to lamps
		for (int i = 0; i < gameObjects.size(); i++)
		{
			GameObject* gameObject = gameObjects.at(i)->As<GameObject>();
			std::string className = gameObject->GetName();

			if (className == "SingleStreetLampPost")
			{
				// Point Lights
				mSpotLights.push_back(new SpotLight(*mGame));
				mSpotLights.at(0)->SetPosition(gameObject->getPosition().x, gameObject->getPosition().y + 5, gameObject->getPosition().z);
				mSpotLights.at(0)->SetRadius(30.0f);
				mSpotLights.at(0)->SetColor(Colors::Purple - SimpleMath::Vector3(0.0f, 0.0f, 0.2f));
				mSpotLights.at(0)->ApplyRotation(XMMatrixRotationX(XMConvertToRadians(-90.0f)));
			}
		}

		for (int i = 0; i < gameObjects.size(); i++)
		{
			GameObject* gameObject = gameObjects.at(i)->As<GameObject>();
			std::string className = gameObject->GetName();

			if (className == "Policeman")
			{
				Policeman* policeman = gameObjects.at(i)->As<Policeman>();

				std::vector<XMFLOAT2> patrolPoints = std::vector<XMFLOAT2>();
				patrolPoints.push_back(XMFLOAT2(-62, -178));
				patrolPoints.push_back(XMFLOAT2(182, -178));
				patrolPoints.push_back(XMFLOAT2(182, -326));
				patrolPoints.push_back(XMFLOAT2(-62, -326));

				policeman->Patrol(patrolPoints);
			}
		}

		Colliders* collD = new Colliders();
		collD->setTriggerReaction(PAINTING_POSITION, { 45.f, -7.f, 22.f }, { 9.f, 2.f, 9.f});
		this->getListOfNode().at(0)->AddTriggerCollider(collD);
	}
}