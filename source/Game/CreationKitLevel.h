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
		const int mSpeedlyIterationFactor = 5;

		// Little work around - size() get wrong value
		const static int mAssetsSize = 27;
		const std::string mAssets[mAssetsSize] =
		{
			"Bench",
			"CargoTrain",
			"ConcreteWall",
			"DoubleStreetLampPostWithMegaphone",
			"FarbaMan",
			"GrassGround",
			"GrassGroundNew",
			"Information",
			"InformationBoard",
			"LongBuilding",
			"OldBuilding",
			"OverheadLines",
			"PassengerTrain",
			"Pathcircles",
			"PlatformGround1",
			"PlatformGround2",
			"PlatformGroundPlain",
			"PlatformNumber1",
			"PlatformNumber2And3",
			"PlatformNumber4",
			"PoliceCar",
			"Policeman",
			"PoliceStation",
			"SingleStreetLampPost",
			"Soldier",
			"StoneGround",
			"Track"
		};
	};
}