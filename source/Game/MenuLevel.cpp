#include "MenuLevel.h"

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
		GreenSoldier* greenSoldier = new GreenSoldier(game, camera, XMFLOAT3());
		this->GameObjects.push_back(greenSoldier);

	}
}

