#include "SerializableGameObject.h"

namespace Library
{
	SerializableGameObject::SerializableGameObject(std::vector<std::string> informations)
	{
		SetParameters(informations);
	}


	SerializableGameObject::~SerializableGameObject()
	{
	}

	void SerializableGameObject::SetParameters(std::vector<std::string> informations)
	{
		if (informations.size() < 10)
			return;

		className = informations.at(0);

		float values[9];
		for (int i = 0; i < 9; i++)
		{
			int informationsIndex = i + 1;
			float value = atof(informations.at(informationsIndex).c_str());
			values[i] = value;
		}

		position = XMFLOAT3(values[0], values[1], values[2]);
		rotation = XMFLOAT3(values[3], values[4], values[5]);
		scale = XMFLOAT3(values[6], values[7], values[8]);
	}
}


