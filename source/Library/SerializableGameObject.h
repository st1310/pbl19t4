#pragma once

#include "Common.h"

namespace Library
{
	class SerializableGameObject
	{
	public:
		SerializableGameObject(std::vector<std::string> informations);
		~SerializableGameObject();

	public:
		std::string className;
		XMFLOAT3 position;
		XMFLOAT3 rotation;
		XMFLOAT3 scale;

	private:
		void SetParameters(std::vector<std::string> informations);
	};
}


