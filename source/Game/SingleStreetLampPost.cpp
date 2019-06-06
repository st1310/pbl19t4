#include "SingleStreetLampPost.h"
#include "SpotLight.h"
#include "AssetList.h"

namespace Rendering
{
	SingleStreetLampPost::SingleStreetLampPost(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"SingleStreetLampPost",
			startPosition,
			startRotation,
			startScale, true)
	{
		mSpotLight = new SpotLight(game);
		mSpotLight->SetRadius(30.0f);
		mSpotLight->SetColor(Colors::Purple - SimpleMath::Vector3(0.0f, 0.0f, 0.2f));
		mSpotLight->ApplyRotation(XMMatrixRotationX(XMConvertToRadians(-90.0f)));
	}


	SingleStreetLampPost::~SingleStreetLampPost()
	{	
		DeleteObject(mSpotLight);
	}

	void SingleStreetLampPost::Initialize()
	{
		StaticGameObject::Initialize();
		StaticGameObject::BuildBoundingBox(XMFLOAT3(2.f, 15.f, 2.f));
	}

	void SingleStreetLampPost::Update(const GameTime& gameTime)
	{
		StaticGameObject::Update(gameTime);
		mSpotLight->SetPosition(mPosition);
	}

	SpotLight* SingleStreetLampPost::GetSpotLight()
	{
		return mSpotLight;
	}
}