#include "MenuLevel.h"
#include "TexturedModelMaterialDemo.h"
#include "Camera.h"
#include "PointLight.h"
#include <WICTextureLoader.h>
#include "GameException.h"

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
			GameObject* gameObject = GameObjects.at(i)->As<GameObject>();
			std::string className = gameObject->GetName();

			if (className == "Soldier")
			{
				// Point Lights
				mPointLights.push_back(new PointLight(*mGame));
				mPointLights.at(0)->SetRadius(30.0f);
				mPointLights.at(0)->SetPosition(gameObject->getPosition().x, gameObject->getPosition().y + 5, gameObject->getPosition().z);
				mPointLights.at(0)->SetColor(Colors::Red - SimpleMath::Vector3(0.0f, 0.0f, 0.5f));
			}
		}

		for (int i = 0; i < gameObjects.size(); i++)
		{
			GameObject* gameObject = GameObjects.at(i)->As<GameObject>();
			gameObject->Initialize();
			gameObject->SetLightsReferences(GetDirectionalLights(), GetPointLights(), GetSpotLights());
		}

		
	}

	void MenuLevel::Update(const GameTime& gameTime) {
		

	}

}