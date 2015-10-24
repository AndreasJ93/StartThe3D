#include "Base Shader Class.h"

#pragma region Constructor and Deconstructor

BaseShaderClass::BaseShaderClass()
{
	mVertexShader = 0;
	mPixelShader = 0;
	mHullShader = 0;
	mComputeShader = 0;
	mDomainShader = 0;
	mGeometryShader = 0;
	mLayout = 0;
	mMatrixBuffer = 0;
	mSampleStateTexture = 0;


}

BaseShaderClass::BaseShaderClass(const BaseShaderClass& original)
{

}

BaseShaderClass::~BaseShaderClass()
{

}

#pragma endregion

#pragma region Initialize and Shutdown

bool BaseShaderClass::Initialize(ID3D11Device* mDevice, HWND handle, WCHAR* VSFilename, WCHAR* GSFilename, WCHAR* PSFilename, WCHAR* CSFilename, WCHAR* HSFilename, WCHAR* DSFilename, char* VSEntryPoint, char* GSEntryPoint, char* PSEntryPoint, char* CSEntryPoint, char* HSEntryPoint, char* DSEntryPoint)
{
	bool result;
	result = InitializeShader(mDevice, handle, VSFilename, GSFilename, PSFilename, CSFilename, HSFilename, DSFilename, VSEntryPoint, GSEntryPoint, PSEntryPoint, CSEntryPoint, HSEntryPoint, DSEntryPoint);

	if (!result)
	{
		return false;
	}


	return true;
}

bool BaseShaderClass::InitializeShader(ID3D11Device* mDevice, HWND handle, WCHAR* VSFilename, WCHAR* GSFilename, WCHAR* PSFilename, WCHAR* CSFilename,  WCHAR* HSFilename, WCHAR* DSFilename, char* VSEntryPoint, char* GSEntryPoint, char* PSEntryPoint, char* CSEntryPoint, char* HSEntryPoint, char* DSEntryPoint)
{
	HRESULT hr;
	ID3D10Blob *errorBlob, *pVS, *pPS, *pGS, *pCS, *pHS, *pDS;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC sampleDesc;

	errorBlob = pVS = pPS = pGS = pCS = pHS = pDS = 0;

	//Compile the vertex shader
	if (VSFilename)
	{
		hr = D3DCompileFromFile(VSFilename, NULL, NULL, VSEntryPoint, "vs_5_0", NULL, NULL, &pVS, &errorBlob);
		if (FAILED(hr))
		{
			//If there is a fail in compiling
			if (errorBlob)
			{
				OutputShaderErrorMessage(errorBlob, handle, VSFilename);
			}
			else //Can't find the file
			{
				MessageBox(handle, VSFilename, L"Missing Shader File", MB_OK);
			}
		}

		hr = mDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), NULL, &mVertexShader);
		if (FAILED(hr))
		{
			return false;
		}
		cout << ">>>>Vertex Shader Initialized" << endl;
	}
	//Compile the geometry shader
	if (GSFilename)
	{

		hr = D3DCompileFromFile(GSFilename, NULL, NULL, GSEntryPoint, "gs_5_0", NULL, NULL, &pGS, &errorBlob);
		if (FAILED(hr))
		{
			//If there is a fail in compiling
			if (errorBlob)
			{
				OutputShaderErrorMessage(errorBlob, handle, GSFilename);
			}
			else //Can't find the file
			{
				MessageBox(handle, GSFilename, L"Missing Shader File", MB_OK);
			}
		}

		hr = mDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), NULL, &mGeometryShader);
		if (FAILED(hr))
		{
			return false;
		}
		cout << ">>>>Geometry Shader Initialized" << endl;
	}
	//Compile the pixel shader
	if (PSFilename)
	{
		hr = D3DCompileFromFile(PSFilename, NULL, NULL, PSEntryPoint, "ps_5_0", NULL, NULL, &pPS, &errorBlob);
		if (FAILED(hr))
		{
			//If there is a fail in compiling
			if (errorBlob)
			{
				OutputShaderErrorMessage(errorBlob, handle, PSFilename);
			}
			else //Can't find the file
			{
				MessageBox(handle, PSFilename, L"Missing Shader File", MB_OK);
			}
		}
		hr = mDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), NULL, &mPixelShader);
		if (FAILED(hr))
		{
			return false;
		}
		cout << ">>>>Pixel Shader Initialized " << endl;

	}

	if (CSFilename)
	{
		hr = D3DCompileFromFile(CSFilename, NULL, NULL, CSEntryPoint, "cs_5_0", NULL, NULL, &pCS, &errorBlob);
		if (FAILED(hr))
		{
			//If there is a fail in compiling
			if (errorBlob)
			{
				OutputShaderErrorMessage(errorBlob, handle, CSFilename);
			}
			else //Can't find the file
			{
				MessageBox(handle, PSFilename, L"Missing Shader File", MB_OK);
			}
		}
		hr = mDevice->CreateComputeShader(pCS->GetBufferPointer(), pCS->GetBufferSize(), NULL, &mComputeShader);
		if (FAILED(hr))
		{
			return false;
		}

		cout << ">>>>Compute Shader Initialzied" << endl;
	}

	if (DSFilename)
	{
		hr = D3DCompileFromFile(DSFilename, NULL, NULL, DSEntryPoint, "ds_5_0", NULL, NULL, &pDS, &errorBlob);
		if (FAILED(hr))
		{
			//If there is a fail in compiling
			if (errorBlob)
			{
				OutputShaderErrorMessage(errorBlob, handle, DSFilename);
			}
			else //Can't find the file
			{
				MessageBox(handle, PSFilename, L"Missing Shader File", MB_OK);
			}
		}

		hr = mDevice->CreateDomainShader(pDS->GetBufferPointer(), pDS->GetBufferSize(), NULL, &mDomainShader);
		if (FAILED(hr))
		{
			return false;
		}

		cout << ">>>>Domain Shader Initialized " << endl;
	}
	
	if (HSFilename)
	{
		hr = D3DCompileFromFile(HSFilename, NULL, NULL, HSEntryPoint, "hs_5_0", NULL, NULL, &pHS, &errorBlob);
		if (FAILED(hr))
		{
			//If there is a fail in compiling
			if (errorBlob)
			{
				OutputShaderErrorMessage(errorBlob, handle, HSFilename);
			}
			else //Can't find the file
			{
				MessageBox(handle, PSFilename, L"Missing Shader File", MB_OK);
			}
		}
		hr = mDevice->CreateHullShader(pHS->GetBufferPointer(), pHS->GetBufferSize(), NULL, &mHullShader);

		cout << ">>>>Hull Shader Initialized" << endl;
	}



	


	//Create the Layout\\

	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//Create the vertex input layout
	hr = mDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &mLayout);
	if (FAILED(hr))
	{
		return false;
	}

	ReleaseCOM(errorBlob);
	ReleaseCOM(pVS);
	ReleaseCOM(pPS);
	ReleaseCOM(pGS);

	//Create the texture sample state
	sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.MipLODBias = 0.0f;
	sampleDesc.MaxAnisotropy = 1;
	sampleDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampleDesc.BorderColor[0] = 0;
	sampleDesc.BorderColor[1] = 0;
	sampleDesc.BorderColor[2] = 0;
	sampleDesc.BorderColor[3] = 0;
	sampleDesc.MinLOD = 0;
	sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = mDevice->CreateSamplerState(&sampleDesc, &mSampleStateTexture);
	if (FAILED(hr))
	{
		return false;
	}

	//Create constant buffers\\

	//Create the buffer description for the matrix
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	hr = mDevice->CreateBuffer(&matrixBufferDesc, NULL, &mMatrixBuffer);
	if (FAILED(hr))
	{
		return false;
	}



	return true;
}

void BaseShaderClass::Shutdown()
{
	ShutdownShader();
}

void BaseShaderClass::ShutdownShader()
{
	ReleaseCOM(mSampleStateTexture);
	ReleaseCOM(mLayout);
	ReleaseCOM(mMatrixBuffer);
	ReleaseCOM(mVertexShader);
	ReleaseCOM(mPixelShader); 
	ReleaseCOM(mGeometryShader);
	ReleaseCOM(mDomainShader);
	ReleaseCOM(mHullShader);
	ReleaseCOM(mComputeShader);

}

void BaseShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorBlob, HWND handle, WCHAR* fileName)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorBlob->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorBlob->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(handle, L"Error compiling shader.  Check shader-error.txt for message.", fileName, MB_OK);

	return;
}

#pragma endregion

#pragma region Render functions

bool BaseShaderClass::Render(ID3D11DeviceContext* mDeviceContext, int indexCount, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture)
{
	bool result;

	result = SetShaderParameters(mDeviceContext, world, view, projection, texture);
	if (!result)
	{
		return false;
	}

	RenderShader(mDeviceContext, indexCount);

	return true;
}

bool BaseShaderClass::SetShaderParameters(ID3D11DeviceContext* mDeviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* matrixPtr;

	XMMATRIX WVP = world*view*projection;


	//Update matrix buffer\\

	hr = mDeviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		return false;
	}

	matrixPtr = (MatrixBufferType*)mappedResource.pData;

	matrixPtr->WVP = XMMatrixTranspose(WVP);
	matrixPtr->world = XMMatrixTranspose(world);
	matrixPtr->view = XMMatrixTranspose(view);
	matrixPtr->projection = XMMatrixTranspose(projection);

	mDeviceContext->Unmap(mMatrixBuffer, 0);

	bufferNumber = 0;

	mDeviceContext->PSSetShaderResources(0, 1, &texture);
	
	mDeviceContext->GSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);


	return true;

}

void BaseShaderClass::RenderShader(ID3D11DeviceContext* mDeviceContext, int indexCount)
{
	mDeviceContext->IASetInputLayout(mLayout);

	mDeviceContext->VSSetShader(mVertexShader, NULL, 0);
	mDeviceContext->GSSetShader(mGeometryShader, NULL, 0);
	mDeviceContext->PSSetShader(mPixelShader, NULL, 0);

	mDeviceContext->PSSetSamplers(0, 1, &mSampleStateTexture);

	mDeviceContext->DrawIndexed(indexCount, 0, 0);

}




#pragma endregion