#include "BoneVertexWeights.h"
#include "GameException.h"

namespace Library
{
	const std::vector<BoneVertexWeights::VertexWeight>& BoneVertexWeights::Weights()
	{
		return mWeights;
	}

	void BoneVertexWeights::AddWeight(float weight, UINT boneIndex)
	{
		if (mWeights.size() == MaxBoneWeightsPerVertex)
		{
			throw GameException("Maximum number of bone weights per vertex exceeded.");
		}

		mWeights.push_back(VertexWeight(weight, boneIndex));
	}
}
