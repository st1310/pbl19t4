#pragma once

#include "Common.h"

namespace Library
{
	class BoneVertexWeights
	{
	public:
		typedef struct _VertexWeight
		{
			float Weight;
			UINT BoneIndex;

			_VertexWeight(float weight, UINT boneIndex) :
				Weight(weight), BoneIndex(boneIndex) { }
		} VertexWeight;

		const std::vector<VertexWeight>& Weights();

		void AddWeight(float weight, UINT boneIndex);

		static const UINT MaxBoneWeightsPerVertex = 4U;

	private:
		std::vector<VertexWeight> mWeights;
	};
}
