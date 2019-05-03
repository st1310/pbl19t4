#include "MenuLevel.h"
#include "TexturedModelMaterialDemo.h"

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
		TexturedModelMaterialDemo* p1 = new TexturedModelMaterialDemo(game, camera, L"Content\\Textures\\checker.dds");
		PassengerTrain* m1 = new PassengerTrain(game, camera);
		GreenSoldier* greenSoldier = new GreenSoldier(game, camera);
		greenSoldier->SetRemovalComponent(true);
		this->GameObjects.push_back(greenSoldier);
		//this->GameObjects.push_back(m1);

	}
}

