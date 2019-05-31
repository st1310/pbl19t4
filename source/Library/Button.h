#pragma once
#include"Common.h"
#include "Game.h"
#include "Camera.h"

namespace Library
{

	class Button {
	public:
		Button(Game& game, Camera& camera, XMFLOAT2 position, XMFLOAT2 size, std::string texturePath);
		~Button();

		void Init();

		XMFLOAT2 GetPosition();
		XMFLOAT2 GetSize();
		ID3D11ShaderResourceView* GetCurrentButtonTexture();

	private:
		XMFLOAT2 position;
		XMFLOAT2 size;
		std::string texture = nullptr;
		ID3D11ShaderResourceView* currentButtonTexture;

		Game* mGame;
		Camera* mCamera;

	};
}
