#include "Button.h"
#include <WICTextureLoader.h>
#include <sstream>

namespace Library
{
	Button::Button(Game& game, Camera& camera, XMFLOAT2 pos, XMFLOAT2 size, std::string texturePath) : mGame(&game), mCamera(&camera),
		position(pos), size(size), texture(texturePath), currentButtonTexture(nullptr)
	{
	}

	Button::~Button() {

	}

	void Button::Init() {

		std::wostringstream textureName;
		textureName << texture.c_str();
		//textureName << L"content\\Textures\\pepeBlack.png";
		HRESULT hr = DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), textureName.str().c_str(), nullptr, &currentButtonTexture);

	}

	XMFLOAT2 Button::GetPosition() {
		return position;
	}

	XMFLOAT2 Button::GetSize() {
		return size;
	}

	ID3D11ShaderResourceView* Button::GetCurrentButtonTexture() {
		return currentButtonTexture;
	}
}


