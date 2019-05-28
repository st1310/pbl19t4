#include "InformationBoard.h"

namespace Rendering
{
	InformationBoard::InformationBoard(Game& game, Camera& camera,
		XMFLOAT3 startPosition,
		XMFLOAT3 startRotation,
		XMFLOAT3 startScale)
		: StaticGameObject(game, camera,
			"InformationBoard",
			"Content\\Models\\InformationBoard.fbx",
			L"Content\\Effects\\TextureMapping.cso",
			"Content\\Textures\\InformationBoardDiffuseMap.jpg",
			startPosition,
			startRotation,
			startScale)
	{
	}


	InformationBoard::~InformationBoard()
	{
	}
}