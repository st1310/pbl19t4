#include "SerializableGameObject.h"

namespace Library
{
	SerializableGameObject::SerializableGameObject()
	{
	}


	SerializableGameObject::~SerializableGameObject()
	{
	}

	std::vector<std::string> SerializableGameObject::Serialize(GameComponent* gameObject)
	{
		std::vector<std::string> result = std::vector<std::string>();

		if (gameObject == NULL)
            return result;

		std::vector<std::string> informations = gameObject->Serialize();		

		std::string tab = "   ";

		result.push_back("<" + informations.at(0) + ">");

		result.push_back(tab + "<Position>");
		result.push_back(tab + tab + "<x= " + GetShortFloat(informations.at(1)) + "/>");
		result.push_back(tab + tab + "<y= " + GetShortFloat(informations.at(2)) + "/>");
		result.push_back(tab + tab + "<z= " + GetShortFloat(informations.at(3)) + "/>");
		result.push_back(tab + "</Position>");

		result.push_back(tab + "<Rotation>");
		result.push_back(tab + tab + "<x= " + GetShortFloat(informations.at(4)) + "/>");
		result.push_back(tab + tab + "<y= " + GetShortFloat(informations.at(5)) + "/>");
		result.push_back(tab + tab + "<z= " + GetShortFloat(informations.at(6)) + "/>");
		result.push_back(tab + "</Rotation>");

		result.push_back(tab + "<Scale>");
		result.push_back(tab + tab + "<x= " + GetShortFloat(informations.at(7)) + "/>");
		result.push_back(tab + tab + "<y= " + GetShortFloat(informations.at(8)) + "/>");
		result.push_back(tab + tab + "<z= " + GetShortFloat(informations.at(9)) + "/>");
		result.push_back(tab + "</Scale>");

		result.push_back("</" + informations.at(0) + ">");

		return result;
	}

	void SerializableGameObject::Deserialize(std::vector<std::string> informations)
	{
		if (informations.size() < DeserializedLinesCount)
			return;

		mAssetClassName = GetValidAssetClassName(informations.at(0));

		float values[9];

		// Position
		values[0] = GetValidFloat(informations.at(2));
		values[1] = GetValidFloat(informations.at(3));
		values[2] = GetValidFloat(informations.at(4));

		// Rotation
		values[3] = GetValidFloat(informations.at(7));
		values[4] = GetValidFloat(informations.at(8));
		values[5] = GetValidFloat(informations.at(9));

		// Rotation
		values[6] = GetValidFloat(informations.at(12));
		values[7] = GetValidFloat(informations.at(13));
		values[8] = GetValidFloat(informations.at(14));

		mPosition = XMFLOAT3(values[0], values[1], values[2]);
		mRotation = XMFLOAT3(values[3], values[4], values[5]);
		mScale = XMFLOAT3(values[6], values[7], values[8]);
	}

	std::string SerializableGameObject::GetAssetClassName()
	{
		return mAssetClassName;
	}

	XMFLOAT3 SerializableGameObject::GetPosition()
	{
		return mPosition;
	}

	XMFLOAT3 SerializableGameObject::GetRotation()
	{
		return mRotation;
	}

	XMFLOAT3 SerializableGameObject::GetScale()
	{
		return mScale;
	}

	std::string SerializableGameObject::GetValidAssetClassName(std::string className)
	{
		int stringSize = className.size();
		std::string result = "";

		for (int i = 0; i < stringSize; i++)
		{
			char character = className.at(i);

			if (character != ' ' &&
				character != '<' &&
				character != '>')
			{
				result += character;
			}
		}

		return result;
	}

	float SerializableGameObject::GetValidFloat(std::string value)
	{
		int stringSize = value.size();
		std::string result = "";

		for (int i = 0; i < stringSize; i++)
		{
			char character = value.at(i);

			if (character != ' ' &&
				character != '<' &&
				character != '>' &&
				character != 'x' &&
				character != 'y' &&
				character != 'z' &&
				character != '/' &&
				character != '=')
			{
				result += character;
			}
		}

		float floatResult = atof(result.c_str());
		return floatResult;
	}

	std::string SerializableGameObject::GetShortFloat(std::string value)
	{
		std::string result = "";

		for (int i = 0; i < value.size(); i++)
		{
			char character = value.at(i);
			result += character;

			if (character == '.')
			{
				if ((i + 1) < value.size())
					result += value.at(i + 1);

				if ((i + 2) < value.size())
					result += value.at(i + 2);

				return result;
			}
		}

		return result;
	}
}


