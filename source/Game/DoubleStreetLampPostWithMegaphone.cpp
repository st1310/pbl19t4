#include "DoubleStreetLampPostWithMegaphone.h"
#include "PointLight.h"
#include "AssetList.h"

namespace Rendering
{
	DoubleStreetLampPostWithMegaphone::DoubleStreetLampPostWithMegaphone(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"DoubleStreetLampPostWithMegaphone",
			startPosition,
			startRotation,
			startScale, true)
	{
		mPointLight = new PointLight(game);
		mPointLight->SetColor(Colors::White - SimpleMath::Vector3(0.0f, 0.0f, 0.2f));
		mPointLight->SetRadius(100.0f);
	}


	DoubleStreetLampPostWithMegaphone::~DoubleStreetLampPostWithMegaphone()
	{
		DeleteObject(mPointLight);
	}

	void DoubleStreetLampPostWithMegaphone::Initialize()
	{
		StaticGameObject::Initialize();
		StaticGameObject::BuildBoundingBox(XMFLOAT3(2.f, 15.f, 2.f));
	}

	void DoubleStreetLampPostWithMegaphone::Update(const GameTime& gameTime)
	{
		StaticGameObject::Update(gameTime);
		XMFLOAT3 pointLightPosition = XMFLOAT3(mPosition.x + 40, mPosition.y + 25, mPosition.z + 7);

		mPointLight->SetPosition(pointLightPosition);
	}

	PointLight* DoubleStreetLampPostWithMegaphone::GetPointLight()
	{
		return mPointLight;
	}
}