#include "DayLevel.h"
#include "Camera.h"

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

		camera.SetPosition(0.0f, 70.0f, 20.0f);

		GameObjectPusher pusher = GameObjectPusher();
		pusher.listNode = this->getListOfNode();
		std::vector<GameComponent*> gameObjects = pusher.CreateAssets(game, camera, LoadFromFile());

		RewriteUnitList(pusher.listOfSoldiers);
		trigerrableObjects = pusher.triggerableObjects;

		for (int i = 0; i < gameObjects.size(); i++)
		{
			this->GameObjects.push_back(gameObjects.at(i));
			GameObjects.at(i)->Initialize();
		}

		for (DrawableGameComponent* drwGm : trigerrableObjects)
		{
			GameObject* gmObj = drwGm->As<GameObject>();
			this->getListOfNode().at(0)->AddTriggerCollider(gmObj->getCollider());
		}
	}
}