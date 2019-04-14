#pragma once

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#define NOMINMAX // This define fixes assimp std::min/max error
//#define UNICODE
//#define STRICT

#include <Windows.h>
#include <exception>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <memory>
//#include <utility>
#include "RTTI.h"

#include <d3d11_1.h>
#include <d3dx11effect.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
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