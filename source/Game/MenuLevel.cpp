#include "MenuLevel.h"
#include "TexturedModelMaterialDemo.h"
#include "Camera.h"
#include "PointLight.h"

namespace Rendering
{
	MenuLevel::MenuLevel(Game& game, Camera& camera)
		:Scene(game, camera, MENU_LEVEL, "Content\\Serializations\\menu_level.data")
	{
	}


	MenuLevel::~MenuLevel()
	{
	}

	void MenuLevel::Start(Game& game, Camera& camera)
	{
		Initialize();
		InitializeLights();
		camera.SetPosition(0.0f, 70.0f, 20.0f);

		GameObjectPusher pusher = GameObjectPusher();
		pusher.listNode = this->getListOfNode();
		std::vector<GameComponent*> gameObjects = pusher.CreateAssets(game, camera, LoadFromFile());

		for (int i = 0; i < gameObjects.size(); i++)
			this->GameObjects.push_back(gameObjects.at(i));

		// Add point Lights to soldiers
		for (int i = 0; i < gameObjects.size(); i++)
		{
			GameObject* gameObject = gameObjects.at(i)->As<GameObject>();
			std::string className = gameObject->GetName();

			if (className == "Soldier")
			{
				GreenSoldier* soldier = gameObjects.at(i)->As<GreenSoldier>();
				mPointLights.push_back(soldier->GetPointLight());
			}
		}

		// Add spot Lights to lamps
		for (int i = 0; i < gameObjects.size(); i++)
		{
			GameObject* gameObject = GameObjects.at(i)->As<GameObject>();
			std::string className = gameObject->GetName();

			if (className == "SingleStreetLampPost")
			{
				SingleStreetLampPost* singleStreetLampPost = gameObjects.at(i)->As<SingleStreetLampPost>();
				mSpotLights.push_back(singleStreetLampPost->GetSpotLight());
			}
		}

		for (int i = 0; i < gameObjects.size(); i++)
		{
			GameObject* gameObject = GameObjects.at(i)->As<GameObject>();
			gameObject->Initialize();
			gameObject->SetLightsReferences(GetDirectionalLights(), GetPointLights(), GetSpotLights());
		}
	}
}