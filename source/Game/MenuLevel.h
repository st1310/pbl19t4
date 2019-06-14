#pragma once

#include "GameObjectPusher.h"

namespace Rendering
{
	class MenuLevel :
		public Scene
	{
	public:
		MenuLevel(Game& game, Camera& camera);
		~MenuLevel();

		virtual void Start(Game& game, Camera& camera) override;
		virtual void Update(const GameTime& gameTime) override;

	private:
		ID3D11ShaderResourceView* mTacticalMap;
	};
}


