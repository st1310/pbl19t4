#include "GUI.h"

namespace Library
{

		GUI::GUI(Game& game, Camera& camera) : DrawableGameComponent(game, camera){

		}

		GUI::~GUI() {

		}

		void GUI::Initialize() {
			for (int i = 0; i < buttonsList.size(); i++) {
				buttonsList.at(i)->Init();
			}
		}

		void GUI::Update(const GameTime& gameTime) {

		}

		void GUI::Draw(const GameTime& gameTime) {

		}

		void GUI::AddButton(Button* button) {
			buttonsList.push_back(button);
		}

		std::vector<Button*> GUI::GetButtonList() {
			return buttonsList;
		}

	
}