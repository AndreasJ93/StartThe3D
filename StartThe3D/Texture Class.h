#ifndef TEXTURECLASS_H
#define TEXTURECLASS_H

#include "Help Functions and Libraries.h"

class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass& original);
	~TextureClass();

	bool Initialize(ID3D11Device* mDevice, ID3D11DeviceContext* mDeviceContext, const WCHAR* textureLocation);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* mTexture;
};





#endif