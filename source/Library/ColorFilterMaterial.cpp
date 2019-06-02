#include "ColorFilterMaterial.h"
#include "GameException.h"

namespace Library
{
	RTTI_DEFINITIONS(ColorFilterMaterial)

		ColorFilterMaterial::ColorFilterMaterial() :PostProcessingMaterial(),
		MATERIAL_VARIABLE_INITIALIZATION(ColorFilter)
	{
	}

	MATERIAL_VARIABLE_DEFINITION(ColorFilterMaterial, ColorFilter)

		void ColorFilterMaterial::Initialize(Effect* effect)
	{
		PostProcessingMaterial::Initialize(effect);

		MATERIAL_VARIABLE_RETRIEVE(ColorFilter)
	}
}