#pragma once

#include "DrawableGameComponent.h"
#include "TexturedModelDemo.h"

using namespace Library;
using namespace Rendering;

class Earth :
	public TexturedModelDemo
{
public:
	Earth(Game& game, Camera& camera);
	~Earth();
};

