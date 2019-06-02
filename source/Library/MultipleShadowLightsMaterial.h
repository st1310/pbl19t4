#pragma once

#include "Common.h"
#include "MultipleLightsMaterial.h"

namespace Library
{
	class MultipleShadowLightsMaterial :
		public MultipleLightsMaterial
	{
		RTTI_DECLARATIONS(MultipleShadowLightsMaterial, MultipleLightsMaterial)

		MATERIAL_VARIABLE_DECLARATION(PointLightProjectiveTexture)

	public:
		MultipleShadowLightsMaterial();

		virtual void Initialize(Effect* effect) override;
	};
}
