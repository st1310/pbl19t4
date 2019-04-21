#include "BoneAnimation.h"
#include "GameException.h"
#include "Bone.h"
#include "Keyframe.h"
#include "Model.h"
#include "VectorHelper.h"
#include <assimp/scene.h>
#include <functional>

namespace Library
{
	BoneAnimation::BoneAnimation(Model& model, aiNodeAnim& nodeAnim) :
		mModel(&model), mBone(nullptr), mKeyframes()
	{
		//std::function<SceneNode*(SceneNode*, std::string)> findByName;
		//findByName = [&findByName](SceneNode* sceneNode, std::string name)->SceneNode* {
		//	if (!sceneNode->Name().compare(name))
		//		return sceneNode;

		//	for (SceneNode* child : sceneNode->Children())
		//		findByName(child, name);
		//	
		//	//throw GameException("nodeAnim.mNodeName does not exist in the model hierarchy!");
		//};

		//mBone = findByName(model.RootNode(), nodeAnim.mNodeName.C_Str());

		mBone = model.SceneNodeByName().at(nodeAnim.mNodeName.C_Str());

		//UINT boneIndex = model.BoneIndexMapping().at(nodeAnim.mNodeName.C_Str());
		//mBone = model.Bones().at(boneIndex);

		/*aiVectorKey* positionKeys = nodeAnim.mPositionKeys;
		aiQuatKey* rotationKeys = nodeAnim.mRotationKeys;
		aiVectorKey* scaleKeys = nodeAnim.mScalingKeys;*/

		UINT maxNumKeys = std::max(nodeAnim.mNumPositionKeys, nodeAnim.mNumRotationKeys);
		maxNumKeys = std::max(maxNumKeys, nodeAnim.mNumScalingKeys);

		aiVectorKey* positionKeys = new aiVectorKey[maxNumKeys];
		aiQuatKey* rotationKeys = new aiQuatKey[maxNumKeys];
		aiVectorKey* scaleKeys = new aiVectorKey[maxNumKeys];

		memcpy(positionKeys, nodeAnim.mPositionKeys, nodeAnim.mNumPositionKeys * sizeof(nodeAnim.mPositionKeys[0]));
		memcpy(rotationKeys, nodeAnim.mRotationKeys, nodeAnim.mNumRotationKeys * sizeof(nodeAnim.mRotationKeys[0]));
		memcpy(scaleKeys, nodeAnim.mScalingKeys, nodeAnim.mNumScalingKeys * sizeof(nodeAnim.mScalingKeys[0]));

		if (nodeAnim.mNumPositionKeys != nodeAnim.mNumRotationKeys ||
			nodeAnim.mNumPositionKeys != nodeAnim.mNumScalingKeys)
		{
			if (nodeAnim.mNumPositionKeys > nodeAnim.mNumRotationKeys)
			{
				for (UINT i = 0; i < nodeAnim.mNumPositionKeys; i++)
				{
					rotationKeys[i] = aiQuatKey(positionKeys[i].mTime, nodeAnim.mRotationKeys[0].mValue);
					scaleKeys[i] = aiVectorKey(positionKeys[i].mTime, nodeAnim.mScalingKeys[0].mValue);
				}
			}
			else if (nodeAnim.mNumRotationKeys > nodeAnim.mNumScalingKeys)
			{
				for (UINT i = 0; i < nodeAnim.mNumRotationKeys; i++)
				{
					positionKeys[i] = aiVectorKey(rotationKeys[i].mTime, nodeAnim.mPositionKeys[0].mValue);
					scaleKeys[i] = aiVectorKey(rotationKeys[i].mTime, nodeAnim.mScalingKeys[0].mValue);
				}
			}
			else if (nodeAnim.mNumScalingKeys > nodeAnim.mNumPositionKeys)
			{
				for (UINT i = 0; i < nodeAnim.mNumScalingKeys; i++)
				{
					rotationKeys[i] = aiQuatKey(scaleKeys[i].mTime, nodeAnim.mRotationKeys[0].mValue);
					positionKeys[i] = aiVectorKey(scaleKeys[i].mTime, nodeAnim.mPositionKeys[0].mValue);
				}
			}
//#if defined( DEBUG ) || defined( _DEBUG )
//			std::wstring s = std::to_wstring(scaleKeys[1].mTime);
//			OutputDebugString((s + L" Sc ").c_str());
//#endif //defined( DEBUG ) || defined( _DEBUG )
		}

		assert(nodeAnim.mNumPositionKeys == nodeAnim.mNumRotationKeys);
		assert(nodeAnim.mNumPositionKeys == nodeAnim.mNumScalingKeys);

		for (UINT i = 0; i < nodeAnim.mNumPositionKeys; i++)
		{
			aiVectorKey& positionKey = positionKeys[i];
			aiQuatKey& rotationKey = rotationKeys[i];
			aiVectorKey& scaleKey = scaleKeys[i];

//#if defined( DEBUG ) || defined( _DEBUG )
//			std::wstring s = std::to_wstring(scaleKeys[i].mTime);
//			OutputDebugString((s + L" ").c_str());
//			s = std::to_wstring(scaleKey.mTime);
//			OutputDebugString((s + L" ").c_str());
//#endif //defined( DEBUG ) || defined( _DEBUG )

			assert(positionKey.mTime == rotationKey.mTime);
			assert(positionKey.mTime == scaleKey.mTime);

			Keyframe* keyframe = new Keyframe(static_cast<float>(positionKey.mTime), XMFLOAT3(positionKey.mValue.x, positionKey.mValue.y, positionKey.mValue.z),
				XMFLOAT4(rotationKey.mValue.x, rotationKey.mValue.y, rotationKey.mValue.z, rotationKey.mValue.w), XMFLOAT3(scaleKey.mValue.x, scaleKey.mValue.y, scaleKey.mValue.z));
			mKeyframes.push_back(keyframe);
		}
	}

	BoneAnimation::~BoneAnimation()
	{
		for (Keyframe* keyframe : mKeyframes)
		{
			delete keyframe;
		}
	}

	SceneNode& BoneAnimation::GetBone()
	{
		return *mBone;
	}

	const std::vector<Keyframe*> BoneAnimation::Keyframes() const
	{
		return mKeyframes;
	}

	UINT BoneAnimation::GetTransform(float time, XMFLOAT4X4& transform) const
	{
		UINT keyframeIndex = FindKeyframeIndex(time);
		Keyframe* keyframe = mKeyframes[keyframeIndex];

		XMStoreFloat4x4(&transform, keyframe->Transform());

		return keyframeIndex;
	}

	void BoneAnimation::GetTransformAtKeyframe(UINT keyframeIndex, XMFLOAT4X4& transform) const
	{
		// Clamp the keyframe
		if (keyframeIndex >= mKeyframes.size())
		{
			keyframeIndex = mKeyframes.size() - 1;
		}

		Keyframe* keyframe = mKeyframes[keyframeIndex];

		XMStoreFloat4x4(&transform, keyframe->Transform());
	}

	void BoneAnimation::GetInteropolatedTransform(float time, XMFLOAT4X4& transform) const
	{
		Keyframe* firstKeyframe = mKeyframes.front();
		Keyframe* lastKeyframe = mKeyframes.back();

		if (time <= firstKeyframe->Time())
		{
			// Specified time is before the start time of the animation, so return the first keyframe
			XMStoreFloat4x4(&transform, firstKeyframe->Transform());
		}
		else if (time >= lastKeyframe->Time())
		{
			// Specified time is after the end time of the animation, so return the last keyframe
			XMStoreFloat4x4(&transform, lastKeyframe->Transform());
		}
		else
		{
			// Interpolate the transform between keyframes
			UINT keyframeIndex = FindKeyframeIndex(time);
			Keyframe* keyframeOne = mKeyframes[keyframeIndex];
			Keyframe* keyframeTwo = mKeyframes[keyframeIndex + 1];

			XMVECTOR translationOne = keyframeOne->TranslationVector();
			XMVECTOR rotationQuaternionOne = keyframeOne->RotationQuaternionVector();
			XMVECTOR scaleOne = keyframeOne->ScaleVector();

			XMVECTOR translationTwo = keyframeTwo->TranslationVector();
			XMVECTOR rotationQuaternionTwo = keyframeTwo->RotationQuaternionVector();
			XMVECTOR scaleTwo = keyframeTwo->ScaleVector();

			float lerpValue = ((time - keyframeOne->Time()) / (keyframeTwo->Time() - keyframeOne->Time()));
			XMVECTOR translation = XMVectorLerp(translationOne, translationTwo, lerpValue);
			XMVECTOR rotationQuaternion = XMQuaternionSlerp(rotationQuaternionOne, rotationQuaternionTwo, lerpValue);
			XMVECTOR scale = XMVectorLerp(scaleOne, scaleTwo, lerpValue);

			static XMVECTOR rotationOrigin = XMLoadFloat4(&Vector4Helper::Zero);
			XMStoreFloat4x4(&transform, XMMatrixAffineTransformation(scale, rotationOrigin, rotationQuaternion, translation));
		}
	}

	UINT BoneAnimation::FindKeyframeIndex(float time) const
	{
		Keyframe* firstKeyframe = mKeyframes.front();
		if (time <= firstKeyframe->Time())
		{
			return 0;
		}

		Keyframe* lastKeyframe = mKeyframes.back();
		if (time >= lastKeyframe->Time())
		{
			return mKeyframes.size() - 1;
		}

		UINT keyframeIndex = 1;

		for (; keyframeIndex < mKeyframes.size() - 1 && time >= mKeyframes[keyframeIndex]->Time(); keyframeIndex++);

		return keyframeIndex - 1;
	}
}
