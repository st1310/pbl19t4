#pragma once

#include "Common.h"

struct aiAnimation;

namespace Library
{
	class Model;
	class Bone;
	class BoneAnimation;
	class SceneNode;

	class AnimationClip
	{
		friend class Model;

	public:
		~AnimationClip();

		const std::string& Name() const;
		float Duration() const;
		float TicksPerSecond() const;
		const std::vector<BoneAnimation*>& BoneAnimations() const;
		const std::map<SceneNode*, BoneAnimation*>& BoneAnimationsByBone() const;
		const UINT KeyframeCount() const;

		UINT GetTransform(float time, Bone& bone, XMFLOAT4X4& transform) const;
		void GetTransforms(float time, std::vector<XMFLOAT4X4>& boneTransforms) const;

		void GetTransformAtKeyframe(UINT keyframe, Bone& bone, XMFLOAT4X4& transform) const;
		void GetTransformsAtKeyframe(UINT keyframe, std::vector<XMFLOAT4X4>& boneTransforms) const;

		void GetInteropolatedTransform(float time, Bone& bone, XMFLOAT4X4& transform) const;
		void GetInteropolatedTransforms(float time, std::vector<XMFLOAT4X4>& boneTransforms) const;

	private:
		AnimationClip(Model& model, aiAnimation& animation);

		AnimationClip();
		AnimationClip(const AnimationClip& rhs);
		AnimationClip& operator=(const AnimationClip& rhs);

		std::string mName;
		float mDuration;
		float mTicksPerSecond;
		std::vector<BoneAnimation*> mBoneAnimations;
		std::map<SceneNode*, BoneAnimation*> mBoneAnimationsByBone;
		UINT mKeyframeCount;
	};
}
