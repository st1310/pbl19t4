#pragma once

#include "GameObjectPusher.h"

namespace Rendering
{
	class CreationKitLevel :
		public Scene
	{
	public:
		CreationKitLevel(Game& game, Camera& camera);
		~CreationKitLevel();

		virtual void Start(Game& game, Camera& camera) override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		bool mEditAsset = false;
		int mCurrentAssetIndex = 0;
		const std::string mAssets[13] =
		{
			"Bench",
			"CargoTrain",
			"ConcreteWall",
			"DoubleStreetLampPostWithMegaphone",
			"GrassGround",
			"GreenSoldier",
			"PassengerTrain",
			"PlatformGround1",
			"PlatformGround2",
			"Policeman",
			"SingleStreetLampPost",
			"StoneGround",
			"Track"
		};
	};
}