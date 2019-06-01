#include "PathFinder_Test.h"

namespace Rendering
{
	PathFinder_Test::PathFinder_Test(Game& game, Camera& camera)
		:Scene(game, camera, PATHFINDER_TEST, "Content\\Serializations\\test_level.data")
	{
		gameObjectPusher = new GameObjectPusher();
	}
	PathFinder_Test::~PathFinder_Test()
	{
	}

	void PathFinder_Test::Start(Game& game, Camera& camera)
	{
		Initialize();
		ClearUnitList();

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


	void PathFinder_Test::Update(const GameTime& gameTime) {


	}

	void PathFinder_Test::Draw(const GameTime& gameTime) {

	}

	

}