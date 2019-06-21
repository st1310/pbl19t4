#pragma once


#include "Game.h"
#include "../Library/Model.h"
#include "Mesh.h"
#include "ModelMaterial.h"

using namespace Library;

namespace Rendering
{
	class ModelList 
	{
	public:
		ModelList(Game& game);
		~ModelList();

		void InsertModel(std::string modelName);
		Model* FindModel(std::string modelName);
	private:
		Game& mGame;
		std::vector<std::pair<std::string, Model*>> modelList;
	};

}