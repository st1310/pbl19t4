#include "TrainLevel.h"
#include "Camera.h"
#include "PointLight.h"


namespace Rendering
{
	TrainLevel::TrainLevel(Game& game, Camera& camera)
		:Scene(game, camera, TRAIN_LEVEL, "Content\\Serializations\\train_level.data")
	{
	}

	TrainLevel::~TrainLevel()
	{
	}

	void TrainLevel::Start(Game& game, Camera& camera)
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

		for (int i = 0; i < GameObjects.size(); i++)
		{
			GameObject* gameObject = GameObjects.at(i)->As<GameObject>();
			gameObject->Initialize();
		}

		for (DrawableGameComponent* drwGm : trigerrableObjects)
		{
			GameObject* gmObj = drwGm->As<GameObject>();
			this->getListOfNode().at(0)->AddTriggerCollider(gmObj->getCollider());
		}

		// Add point Lights to soldiers
		for (int i = 0; i < gameObjects.size(); i++)
		{
			GameObject* gameObject = GameObjects.at(i)->As<GameObject>();
			std::string className = gameObject->GetName();

			if (className == "Soldier")
			{
				GreenSoldier* soldier = gameObjects.at(i)->As<GreenSoldier>();
				mPointLights.push_back(soldier->GetPointLight());
			}

			if (className == "Policeman")
			{
				Policeman* policeman = gameObjects.at(i)->As<Policeman>();
				GetPolicemans().push_back(policeman);

				mPointLights.push_back(policeman->GetPointLight());
			}

			if (className == "SingleStreetLampPost")
			{
				SingleStreetLampPost* singleStreetLampPost = gameObjects.at(i)->As<SingleStreetLampPost>();
				//mSpotLights.push_back(singleStreetLampPost->GetSpotLight());
				mPointLights.push_back(singleStreetLampPost->GetPointLight());
			}

			if (className == "DoubleStreetLampPostWithMegaphone")
			{
				DoubleStreetLampPostWithMegaphone* doubleStreetLampPostWithMegaphone = gameObjects.at(i)->As<DoubleStreetLampPostWithMegaphone>();
				//mSpotLights.push_back(doubleStreetLampPostWithMegaphone->GetSpotLight());
				mPointLights.push_back(doubleStreetLampPostWithMegaphone->GetPointLight());
			}
		}


		for (int i = 0; i < GameObjects.size(); i++)
		{
			GameObject* gameObject = GameObjects.at(i)->As<GameObject>();
			gameObject->SetLightsReferences(GetDirectionalLights(), GetPointLights(), GetSpotLights());
			gameObject->Initialize();
		}

		Colliders* collD = new Colliders();
		collD->setTriggerReaction(PAINTING_POSITION, { 45.f, -7.f, 22.f }, { 9.f, 2.f, 9.f });
		this->getListOfNode().at(0)->AddTriggerCollider(collD);

		//SetPatrol();
	}

	void TrainLevel::SetPatrol()
	{
		std::vector<XMFLOAT2> patrolPoints1 = std::vector<XMFLOAT2>();
		patrolPoints1.push_back(XMFLOAT2(-62, -178));
		patrolPoints1.push_back(XMFLOAT2(182, -178));
		patrolPoints1.push_back(XMFLOAT2(182, -326));
		patrolPoints1.push_back(XMFLOAT2(-62, -326));

		std::vector<XMFLOAT2> patrolPoints2 = std::vector<XMFLOAT2>();
		patrolPoints1.push_back(XMFLOAT2(-62, -178));
		patrolPoints1.push_back(XMFLOAT2(182, -178));
		patrolPoints1.push_back(XMFLOAT2(182, -326));
		patrolPoints1.push_back(XMFLOAT2(-62, -326));

		GetPolicemans().at(2)->As<Policeman>()->Patrol(patrolPoints1);
		GetPolicemans().at(3)->As<Policeman>()->Patrol(patrolPoints1);
		GetPolicemans().at(4)->As<Policeman>()->Patrol(patrolPoints1);
		GetPolicemans().at(5)->As<Policeman>()->Patrol(patrolPoints1);

		GetPolicemans().at(6)->As<Policeman>()->Patrol(patrolPoints1);
		GetPolicemans().at(7)->As<Policeman>()->Patrol(patrolPoints1);
	}
}