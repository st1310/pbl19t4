#include "MenuLevel.h"
#include "TexturedModelMaterialDemo.h"
#include "Camera.h"

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

		camera.SetPosition(0.0f, 70.0f, 20.0f);

		GameObjectPusher pusher = GameObjectPusher();
		pusher.listNode = this->getListOfNode();
		std::vector<GameComponent*> gameObjects = pusher.CreateAssets(game, camera, LoadFromFile());

		for (int i = 0; i < gameObjects.size(); i++)
		{
			this->GameObjects.push_back(gameObjects.at(i));
			GameObjects.at(i)->Initialize();
		}
	}
}