#include "Texture Class.h"

#pragma region Constructor and Deconstructor

TextureClass::TextureClass()
{
	mTexture = nullptr;
}

TextureClass::TextureClass(const TextureClass& original)
{

}

TextureClass::~TextureClass()
{

}


#pragma endregion

#pragma region Initialize and Shutdown

bool TextureClass::Initialize(ID3D11Device* mDevice, ID3D11DeviceContext* mDeviceContext, const WCHAR* textureLocation)
{
	HRESULT hr;

	hr = CreateWICTextureFromFile(mDevice, mDeviceContext, textureLocation, nullptr, &mTexture, 0);

	if (FAILED(hr))
	{
		return false;
	}

	return true;

}


void TextureClass::Shutdown()
{
	ReleaseCOM(mTexture);
}

#pragma endregion

#pragma region Get Functions

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return mTexture;
}

#pragma endregion