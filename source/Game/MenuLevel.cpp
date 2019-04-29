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
		//Button* playButton = new Button(game, camera, XMFLOAT3());
		GreenSoldier* greenSoldier = new GreenSoldier(game, camera);
		greenSoldier->SetRemovalComponent(true);
		this->GameObjects.push_back(greenSoldier);

	}
}

