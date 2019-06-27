#include "Trace1.h"

namespace Rendering
{
	Trace1::Trace1(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"Trace1",
			startPosition,
			startRotation,
			startScale)
	{
	}

	Trace1::~Trace1()
	{
	}

	void Trace1::Initialize()
	{
		StaticGameObject::Initialize();
		this->mCollider->setTriggerReaction(PAINT, mPosition, {12.f, 12.f, 12.f });
	}
}