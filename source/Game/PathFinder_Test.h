#pragma once

#include "GameObjectPusher.h"
#include "PathFinding.h"

namespace Rendering
{
	class PathFinder_Test :
		public Scene
	{
	public:
		PathFinder_Test(Game& game, Camera& camera);
		~PathFinder_Test();

		PathFinding* pathfinding;
		GameObjectPusher* gameObjectPusher;

		virtual void Start(Game& game, Camera& camera) override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		int indexNode = 0;
		bool mChooseEndNode;
	};
}