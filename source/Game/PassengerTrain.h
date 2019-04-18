#pragma once

#include "DrawableGameComponent.h"
#include "TexturedModelDemo.h"

using namespace Library;
using namespace Rendering;

class PassengerTrain :
	public TexturedModelDemo
{
public:
	PassengerTrain(Game& game, Camera& camera);
	~PassengerTrain();
};

