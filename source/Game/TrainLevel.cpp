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

		int policemanSize = 0;

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

			if (className == "FarbaMan") {
				FarbaMan* farbaMan = gameObjects.at(i)->As<FarbaMan>();
				mFarbaMan = farbaMan;
			}

			if (className == "Policeman")
			{
				Policeman* policeman = gameObjects.at(i)->As<Policeman>();
				GetPolicemans().push_back(policeman);

				SetPatrol(policeman, policemanSize);
				policemanSize++;

				mPointLights.push_back(policeman->GetPointLight());
			}

			if (className == "SingleStreetLampPost")
			{
				SingleStreetLampPost* singleStreetLampPost = gameObjects.at(i)->As<SingleStreetLampPost>();
				mPointLights.push_back(singleStreetLampPost->GetPointLight());
			}

			if (className == "DoubleStreetLampPostWithMegaphone")
			{
				DoubleStreetLampPostWithMegaphone* doubleStreetLampPostWithMegaphone = gameObjects.at(i)->As<DoubleStreetLampPostWithMegaphone>();
				mPointLights.push_back(doubleStreetLampPostWithMegaphone->GetPointLight());
			}
 		}		

		for (int i = 0; i < GameObjects.size(); i++)
		{
			GameObject* gameObject = GameObjects.at(i)->As<GameObject>();
			gameObject->SetLightsReferences(GetDirectionalLights(), GetPointLights(), GetSpotLights());
		}

		Colliders* collD = new Colliders();
		collD->setTriggerReaction(PAINTING_POSITION, { 45.f, -7.f, 22.f }, { 9.f, 2.f, 9.f });
		this->getListOfNode().at(0)->AddTriggerCollider(collD);		
	}

	void TrainLevel::SetPatrol(Policeman* policeman, int index)
	{
		std::vector<XMFLOAT2> patrolPoints1 = std::vector<XMFLOAT2>();
		patrolPoints1.push_back(XMFLOAT2(183, -205));
		patrolPoints1.push_back(XMFLOAT2(183, -321));
		patrolPoints1.push_back(XMFLOAT2(6, -321));
		patrolPoints1.push_back(XMFLOAT2(6, -282));
		patrolPoints1.push_back(XMFLOAT2(-51, -282));
		patrolPoints1.push_back(XMFLOAT2(-51, -183));
		patrolPoints1.push_back(XMFLOAT2(91, -183));
		patrolPoints1.push_back(XMFLOAT2(91, -205));

		std::vector<XMFLOAT2> patrolPoints2 = std::vector<XMFLOAT2>();
		patrolPoints2.push_back(XMFLOAT2(215, -220));
		patrolPoints2.push_back(XMFLOAT2(215, 231));
		patrolPoints2.push_back(XMFLOAT2(231, 231));
		patrolPoints2.push_back(XMFLOAT2(231, -220));

		std::vector<XMFLOAT2> patrolPoints3 = std::vector<XMFLOAT2>();
		patrolPoints3.push_back(XMFLOAT2(760, -424));
		patrolPoints3.push_back(XMFLOAT2(760, 20));
		patrolPoints3.push_back(XMFLOAT2(804, 20));
		patrolPoints3.push_back(XMFLOAT2(804, -424));

		if (index == 2 || index == 3 || index == 4 || index == 5)
			policeman->Patrol(patrolPoints1);

		if (index == 6 || index == 7)
			policeman->Patrol(patrolPoints2);

		if (index == 8 || index == 9)
			policeman->Patrol(patrolPoints3);
	}

	FarbaMan* TrainLevel::GetFarbaMan() {
		return mFarbaMan;
	}
}