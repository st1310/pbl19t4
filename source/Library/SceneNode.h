#pragma once

#include "Common.h"

namespace Library
{
	class SceneNode :
		public RTTI
	{
		RTTI_DECLARATIONS(SceneNode, RTTI)

	public:
		SceneNode(const std::string& name);
		SceneNode(const std::string& name, const XMFLOAT4X4& transform);

		const std::string& Name() const;
		SceneNode* ParentNode();
		std::vector<SceneNode*>& Children();
		const XMFLOAT4X4& Transform() const;
		XMMATRIX TransformMatrix() const;

		void SetParent(SceneNode* parent);

		void SetTransform(XMFLOAT4X4& transform);
		void SetTransform(CXMMATRIX transform);

	protected:
		std::string mName;
		SceneNode* mParent;
		std::vector<SceneNode*> mChildren;
		XMFLOAT4X4 mTransform;

	private:
		SceneNode();
		SceneNode(const SceneNode& rhs);
		SceneNode& operator=(const SceneNode& rhs);
	};
}
