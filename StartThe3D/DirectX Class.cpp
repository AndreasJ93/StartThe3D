#include "DirectX Class.h"

#pragma region Constructor and Deconstructor

DirectXClass::DirectXClass()
{
	mSwapChain = 0;
	mDevice = 0;
	mDeviceContext = 0;
	mRenderTargetView = 0;
	mDepthStencilBuffer = 0;
	mDepthStencilState = 0;
	mDepthStencilView = 0;
	mRasterState = 0;

}



DirectXClass::~DirectXClass()
{

}

#pragma endregion

#pragma region Initialize and Shutdown

bool DirectXClass::Initialize(int width, int height, bool vSync, HWND handle, bool fullScreen, float screenDepth, float screenNear)
{
	HRESULT hr;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	mVsyncEnabled = vSync;

	//Swap Chain\\

	//Swap Chain Description
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	swapChainDesc.OutputWindow = handle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	//Fullscreen or window
	if (fullScreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mSwapChain,
		&mDevice,
		NULL,
		&mDeviceContext);
	if (FAILED(hr))
	{
		return false;
	}

	//Fix the backbuffer\\

	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(hr))
	{
		return false;
	}

	hr = mDevice->CreateRenderTargetView(backBufferPtr, NULL, &mRenderTargetView);
	if (FAILED(hr))
	{
		return false;
	}

	//Release the pointer to the back buffer
	ReleaseCOM(backBufferPtr);

	//Depth Buffer\\
		//Description
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;						//Should be the same as in the swapChain!
	depthBufferDesc.SampleDesc.Quality = 0;						//Should be the same as in the swapChain!
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//Create the depth buffer
	hr = mDevice->CreateTexture2D(&depthBufferDesc, NULL, &mDepthStencilBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	//Create the depth stencil description
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Create the depth stencil state
	hr = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	if (FAILED(hr))
	{
		return false;
	}

	//Set the depth stencil state
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

	//Create the depth stencil view description
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//Create the depth stencil view
	hr = mDevice->CreateDepthStencilView(mDepthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView);
	if (FAILED(hr))
	{
		return false;
	}

	//Bind the target view and depth stencil to the pipeline
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	//Create the rasterizer description
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	//Create the rasterizer state
	hr = mDevice->CreateRasterizerState(&rasterDesc, &mRasterState);
	if (FAILED(hr))
	{
		return false;
	}

	//Set the rasterizer state
	mDeviceContext->RSSetState(mRasterState);

	//Viewport\\
		//Set the viewport
	viewport.Height = (float)height;
	viewport.Width = (float)width;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	mDeviceContext->RSSetViewports(1, &viewport);


	//WICTexture Intializing\\

	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		return false;
	}



	//Matrixes\\
		//Create a projection matrix
	fieldOfView = XM_PI * 0.45f;
	screenAspect = (float)width / height;

	XMStoreFloat4x4(&mProjectionMatrix, XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth));

	//Create a worldMatrix
	XMStoreFloat4x4(&mWorldMatrix, XMMatrixIdentity());

	//Create and orthographic projection matrix
	XMStoreFloat4x4(&mOrthoMatrix, XMMatrixOrthographicLH((float)width, (float)height, screenNear, screenDepth));

	return true;
}

void DirectXClass::Shutdown()
{

	//Remove fullscreen
	if (mSwapChain)
	{
		mSwapChain->SetFullscreenState(false, NULL);
	}

	

	ReleaseCOM(mRasterState);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mDepthStencilState);
	ReleaseCOM(mDepthStencilBuffer);
	ReleaseCOM(mRenderTargetView);
	
	ReleaseCOM(mSwapChain);
	ReleaseCOM(mDeviceContext);
	ReleaseCOM(mDevice);


}

#pragma endregion

#pragma region Rendering

void DirectXClass::BeginScene()
{
	float colour[] = { 0.0f, 0.0f, 0.0f, 0.0f };


	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, colour);
}

void DirectXClass::EndScene()
{
	

	if (mVsyncEnabled)
	{
		mSwapChain->Present(1, 0);
	}
	else
	{
		mSwapChain->Present(0, 0);
	}



}

#pragma endregion

#pragma region Get and Set functions

ID3D11Device* DirectXClass::GetDevice()
{
	return mDevice;
}


ID3D11DeviceContext* DirectXClass::GetDeviceContext()
{
	return mDeviceContext;
}

XMMATRIX DirectXClass::GetProjectionMatrix()
{
	return XMLoadFloat4x4(&mProjectionMatrix); 
}

XMMATRIX DirectXClass::GetWorldMatrix()
{
	return XMLoadFloat4x4(&mWorldMatrix);
}

XMMATRIX DirectXClass::GetOrthoMatrix()
{
	return XMLoadFloat4x4(&mOrthoMatrix);
}

#pragma endregion