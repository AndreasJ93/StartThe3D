#ifndef LIGHTSHADERCLASS_H
#define LIGHTSHADERCLASS_H

#include "Structs.h"
#include "Help Functions and Libraries.h"



class BaseShaderClass
{
public:
	BaseShaderClass();
	BaseShaderClass(const BaseShaderClass& original);
	~BaseShaderClass();

	bool Initialize(ID3D11Device* mDevice, HWND handle, WCHAR* VSFilename, WCHAR* GSFilename, WCHAR* PSFilename, WCHAR* CSFilename, WCHAR* HSFilename, WCHAR* DSFilename, char* VSEntryPoint, char* GSEntryPoint, char* PSEntryPoint, char* CSEntryPoint, char* HSEntryPoint, char* DSEntryPoint);
	void Shutdown();
	bool Render(ID3D11DeviceContext* mDeviceContext, int indexCount, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture);

private:

	//Functions
	bool InitializeShader(ID3D11Device* mDevice, HWND handle, WCHAR* VSFilename, WCHAR* GSFilename, WCHAR* PSFilename, WCHAR* CSFilename, WCHAR* HSFilename, WCHAR* DSFilename, char* VSEntryPoint, char* GSEntryPoint, char* PSEntryPoint, char* CSEntryPoint, char* HSEntryPoint, char* DSEntryPoint);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorBlob, HWND handle, WCHAR* fileName);

	bool SetShaderParameters(ID3D11DeviceContext* mDeviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture);
	void RenderShader(ID3D11DeviceContext* mDeviceContext, int indexCount);

	//Variables Rendering
	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11GeometryShader* mGeometryShader;
	ID3D11ComputeShader* mComputeShader;
	ID3D11HullShader* mHullShader;
	ID3D11DomainShader* mDomainShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer* mMatrixBuffer;
	
	ID3D11SamplerState* mSampleStateTexture;

};

#endif