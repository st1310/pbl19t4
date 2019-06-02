#include "MultipleShadowLightsMaterial.h"

namespace Library
{
	RTTI_DEFINITIONS(MultipleShadowLightsMaterial)

	MultipleShadowLightsMaterial::MultipleShadowLightsMaterial() :
		MultipleLightsMaterial(),
		MATERIAL_VARIABLE_INITIALIZATION(PointLightProjectiveTexture)
	{
	}

	MATERIAL_VARIABLE_DEFINITION(MultipleShadowLightsMaterial, PointLightProjectiveTexture)

	void MultipleShadowLightsMaterial::Initialize(Effect* effect)
	{
		MultipleLightsMaterial::Initialize(effect);

		MATERIAL_VARIABLE_RETRIEVE(PointLightProjectiveTexture)
	}
}
