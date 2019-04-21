#include "MenuLevel.h"

const int MENU_LEVEL = 0;


namespace Rendering
{
	MenuLevel::MenuLevel(Game& game, Camera& camera)
		:Scene(MENU_LEVEL)
	{
		Size = 1;
		Start(game, camera);
	}


	MenuLevel::~MenuLevel()
	{
	}

	void MenuLevel::Start(Game& game, Camera& camera)
	{
		Earth* earth1 = new Earth(game, camera, 0, 0);
		this->GameObjects.push_back(earth1);

	}
}

