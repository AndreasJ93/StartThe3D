#ifndef DIRECTXCLASS_H
#define DIRECTXCLASS_H
#include "Help Functions and Libraries.h"

class DirectXClass
{
public:
	DirectXClass();
	~DirectXClass();
	
	bool Initialize(int width, int height, bool vSync, HWND handle, bool fullScreen, float screenDepth, float screenNear);
	void BeginScene();
	void EndScene();
	void Shutdown();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	XMMATRIX GetProjectionMatrix();
	XMMATRIX GetWorldMatrix();
	XMMATRIX GetOrthoMatrix();

private:
	bool mVsyncEnabled;

	IDXGISwapChain* mSwapChain;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;

	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilState* mDepthStencilState;
	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11RasterizerState* mRasterState;

	XMFLOAT4X4 mProjectionMatrix;
	XMFLOAT4X4 mWorldMatrix;
	XMFLOAT4X4 mOrthoMatrix;

};




#endif