#include "ModelList.h"

namespace Rendering
{
		ModelList::ModelList(Game& game) : mGame(game)
		{}

		ModelList::~ModelList()
		{
			modelList.clear();
		}

		void ModelList::InsertModel(std::string newModelName)
		{
			std::string modelName = "Content\\Models\\" + (std::string)newModelName + ".fbx";

			Model* newModel = new Model(mGame, modelName, true);
			modelList.push_back({ newModelName, newModel });
		}

		Model* ModelList::FindModel(std::string modelName)
		{
			for (int i = 0; i < modelList.size(); i++)
			{
				if (modelList[i].first == modelName)
				{
					return modelList[i].second;
				}
			}

			InsertModel(modelName);
			return modelList[modelList.size() - 1].second;
		}
}