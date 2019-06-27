#pragma once

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#define MENU_LEVEL 0
#define DAY_LEVEL 1
#define TRAIN_LEVEL 2
#define CITY_LEVEL 3
#define CREATION_KIT_LEVEL 4
#define PATHFINDER_TEST 5

// For CreationKit
#define ROTATION "Rotation"
#define SCALE "Scale"
#define POSITION "Position"

#define X_AXIS "X"
#define Y_AXIS "Y"
#define Z_AXIS "Z"
#define ALL_AXIS "All"

#define X_AXIS_NUMBER 1
#define Y_AXIS_NUMBER 2
#define Z_AXIS_NUMBER 3
#define ALL_AXIS_NUMBER 4

// Class names in serialization
#define FARBA_MAN_ID 100

#define NOMINMAX // This define fixes assimp std::min/max error
//#define UNICODE
//#define STRICT

#include <Windows.h>
#include <exception>
#include <cassert>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <memory>
#include <algorithm>
//#include <utility>
#include "RTTI.h"

#include <d3d11_1.h>
#include <d3dx11effect.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <SimpleMath.h>
#include "dinput.h"


#define DeleteObject(object) if((object) != NULL) { delete object; object = NULL; }
#define DeleteObjects(objects) if((objects) != NULL) {delete[] objects; objects = NULL; }
#define ReleaseObject(object) if((object) != NULL) {object->Release(); object = NULL; }

namespace Library
{
	typedef unsigned char byte;
}

using namespace DirectX;
using namespace DirectX::PackedVector;