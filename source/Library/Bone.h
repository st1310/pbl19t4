#pragma once

#include "Common.h"
#include "SceneNode.h"

namespace Library
{
	class Bone :
		public SceneNode
	{
		RTTI_DECLARATIONS(Bone, SceneNode)

	public:
		Bone(const std::string& name, UINT index, const XMFLOAT4X4& offsetTransform);

		UINT Index() const;
		void SetIndex(UINT index);

		const XMFLOAT4X4& OffsetTransform() const;
		XMMATRIX OffsetTransformMatrix() const;

	private:
		Bone();
		Bone(const Bone& rhs);
		Bone& operator=(const Bone& rhs);

		UINT mIndex;					// Index into the model's bone container
		XMFLOAT4X4 mOffsetTransform;	// Transforms from mesh space to bone space
	};
}
