#pragma once

#include "Common.h"
#include "GameComponent.h"

namespace Library
{
	class SerializableGameObject
	{
	public:
		SerializableGameObject();
		~SerializableGameObject();

		std::vector<std::string> Serialize(GameComponent* gameObject);
		void Deserialize(std::vector<std::string> informations);

		std::string GetAssetClassName();
		XMFLOAT3 GetPosition();
		XMFLOAT3 GetRotation();
		XMFLOAT3 GetScale();

		const static int DeserializedLinesCount = 17;
	
	private:		
		std::string mAssetClassName;
		XMFLOAT3 mPosition;
		XMFLOAT3 mRotation;
		XMFLOAT3 mScale;		

	private:
		std::string GetValidAssetClassName(std::string className);
		float GetValidFloat(std::string value);
		std::string GetShortFloat(std::string value);
	};
}


