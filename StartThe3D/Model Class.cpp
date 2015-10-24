#include "Model Class.h"

#pragma region Constructor and Deconstructor

ModelClass::ModelClass()
{
	mVertexCount = 0;
	mIndexCount = 0;
	mTexture = 0;
	mModel = 0;
	mNumberOfInstances = 0;
	mGotTexture = false;
}

ModelClass::ModelClass(const ModelClass& original)
{

}

ModelClass::~ModelClass()
{

}

#pragma endregion

#pragma region Initialize and Shutdown

bool ModelClass::Initialize(ID3D11Device* mDevice, ID3D11DeviceContext* mDeviceContext, WCHAR* textureFileName, char* modelFileName)
{
	bool result;

	int vertexCount, textureCount, normalCount, faceCount;

	result = ReadFileCounts(modelFileName, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		return false;
	}
	result = LoadDataStructures(modelFileName, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		return false;
	}

	result = InitializeBuffers(mDevice);
	if (!result)
	{
		return false;
	}

	if (!mGotTexture)
	{
		result = LoadTexture(mDevice, mDeviceContext, textureFileName);
	}
	else
	{
		result = LoadTexture(mDevice, mDeviceContext, mTextureString.c_str());
	}
	if (!result)
	{
		return false;
	}
	
	return true;
}

bool ModelClass::InitializeBuffers(ID3D11Device* mDevice)
{
	VertexTypeLight* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT hr;

	vertices = new VertexTypeLight[mVertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[mIndexCount];
	if (!indices)
	{
		return false;
	}

	for (int i = 0; i < mVertexCount; i++)
	{
		vertices[i].position = mModel[i].position;
		vertices[i].texture = mModel[i].textureCoordinates;
		vertices[i].normal = mModel[i].normal;

		indices[i] = i;
	}

	//Vertex Buffer\\

	//Set Description and Data
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = sizeof(VertexTypeLight)*mVertexCount;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Create the Vertex buffer
	hr = mDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	//Index Buffer\\

	//Set description and Data
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)*mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create the Index buffer
	hr = mDevice->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	delete[] vertices;
	delete[] indices;
	vertices = 0;
	indices = 0;

	return true;

}

void ModelClass::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
	ReleaseModel();
}


void ModelClass::ShutdownBuffers()
{
	ReleaseCOM(mVertexBuffer);
	ReleaseCOM(mIndexBuffer);

}

#pragma endregion

#pragma region Load and unload textures and models

bool ModelClass::LoadTexture(ID3D11Device* mDevice, ID3D11DeviceContext* mDeviceContext, const WCHAR* fileName)
{
	bool result;

	mTexture = new TextureClass;
	if (!mTexture)
	{
		return false;
	}

	result = mTexture->Initialize(mDevice, mDeviceContext, fileName);
	if (!result)
	{
		return false;
	}

	return true;
}


void ModelClass::ReleaseTexture()
{
	mTexture->Shutdown();
}

bool ModelClass::LoadModel(char* fileName)
{
	ifstream fin;
	char input;
	int i;

	fin.open(fileName);
	if (fin.fail())
	{
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> mVertexCount;
	mIndexCount = mVertexCount;

	mModel = new ModelPTN[mVertexCount];

	if (!mModel)
	{
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin.get(input);
	fin.get(input);

	for (i = 0; i < mVertexCount; i++)
	{
		fin >> mModel[i].position.x >> mModel[i].position.y >> mModel[i].position.z;
		fin >> mModel[i].textureCoordinates.x >> mModel[i].textureCoordinates.y;
		fin >> mModel[i].normal.x >> mModel[i].normal.y >> mModel[i].normal.z;
	}

	fin.close();

	return true;
}

void ModelClass::ReleaseModel()
{
	if (mModel)
	{
		delete[] mModel;
		mModel = 0;
	}
}

bool ModelClass::ReadFileCounts(char* fileName, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
{
	ifstream fin;
	char input;

	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	fin.open(fileName);
	if (fin.fail())
	{
		return false;
	}

	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ')
			{
				vertexCount++;
			}
			if (input == 't')
			{
				textureCount++;
			}
			if (input == 'n')
			{
				normalCount++;
			}
		}
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				faceCount++;
			}
		}

		while (input != '\n')
		{
			fin.get(input);
		}

		fin.get(input);
	}

	fin.close();

	return true;

}

bool ModelClass::LoadDataStructures(char* fileName, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	XMFLOAT3 *vertices, *texcoords, *normals;
	ModelFaces *faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;

	vertices = new XMFLOAT3[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new XMFLOAT3[textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new XMFLOAT3[normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new ModelFaces[faceCount];

	vertexIndex = texcoordIndex = normalIndex = faceIndex = 0;

	fin.open(fileName);

	if (fin.fail())
	{
		return false;
	}

	//Note that this code transform the vertices to left handed coordinate system
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'm')
		{
			fin.get(input);
			if (input == 't')
			{
				fin.get(input);
				if (input == 'l')
				{
					fin.get(input);
					if (input == 'l')
					{
						fin.get(input);
						if (input == 'i')
						{
							fin.get(input);
							if (input == 'b')
							{
								ifstream mtlFin;
								string mtlFileName = "";
								fin.get(input);
								fin >> mtlFileName;
								mtlFin.open("Models\\" + mtlFileName);
								char mtlInput;
								if (mtlFin.fail())
								{
									return false;
								}
								mtlFin.get(mtlInput);
								while (!mtlFin.eof())
								{
									if (mtlInput == 'm')
									{
										mtlFin.get(mtlInput);
										if (mtlInput == 'a')
										{
											mtlFin.get(mtlInput);
											if (mtlInput == 'p')
											{
												mtlFin.get(mtlInput);
												if (mtlInput == '_')
												{
													mtlFin.get(mtlInput);
													if (mtlInput == 'K')
													{
														mtlFin.get(mtlInput);
														if (mtlInput == 'd')
														{
															mtlFin.get(mtlInput);
															wstring textureName = L"";
															mtlFin.get(mtlInput);
															while (mtlInput != '\n')
															{
																textureName += mtlInput;
																mtlFin.get(mtlInput);
															}
															mGotTexture = true;
															mTextureString = L"Textures\\" + textureName;

														}
													}
												}
											}

										}
									}
									while (mtlInput != '\n')
									{
										mtlFin.get(mtlInput);
									}
									mtlFin.get(mtlInput);
								}
							}
						}
					}
				}
			}
		}

		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				//Invert to left handed
				vertices[vertexIndex].z = vertices[vertexIndex].z * - 1.0f;
				vertexIndex++;
			}
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				//Invert to left handed
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				//Invert to left handed
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				//Invert to left handed by reading it backwards
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
					>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		while (input != '\n')
		{
			fin.get(input);
		}

		fin.get(input);
	}

	mIndexCount = mVertexCount = faceCount * 3;
	mModel = new ModelPTN[mVertexCount];

	for (int i = 0; i < faceCount; i++)
	{

		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		mModel[i * 3].position = vertices[vIndex];
		mModel[i * 3].textureCoordinates.x = texcoords[tIndex].x;
		mModel[i * 3].textureCoordinates.y = texcoords[tIndex].y;
		mModel[i * 3].normal = normals[nIndex];

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		mModel[i * 3 + 1].position = vertices[vIndex];
		mModel[i * 3 + 1].textureCoordinates.x = texcoords[tIndex].x;
		mModel[i * 3 + 1].textureCoordinates.y = texcoords[tIndex].y;
		mModel[i * 3 + 1].normal = normals[nIndex];

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		mModel[i * 3 + 2].position = vertices[vIndex];
		mModel[i * 3 + 2].textureCoordinates.x = texcoords[tIndex].x;
		mModel[i * 3 + 2].textureCoordinates.y = texcoords[tIndex].y;
		mModel[i * 3 + 2].normal = normals[nIndex];

	}

	mBoundingBox.CreateFromPoints(mBoundingBox, vertexCount, vertices, sizeof(XMFLOAT3));


	return true;
}

#pragma endregion

#pragma region Render Functions

void ModelClass::Render(ID3D11DeviceContext* mDeviceContext)
{
	RenderBuffers(mDeviceContext);
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* mDeviceContext)
{

	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexTypeLight);
	offset = 0;

	mDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	mDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

#pragma endregion

#pragma region Get and Set functions

int ModelClass::GetIndexCount()
{
	return mIndexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return mTexture->GetTexture();
}

XMMATRIX ModelClass::GetWorldMatrix()
{
	return DirectX::XMLoadFloat4x4(&mWorldMatrix);
}
XMMATRIX ModelClass::GetInverseWorldMatrix()
{
	return DirectX::XMLoadFloat4x4(&mInverseWorldMatrix);
}

int ModelClass::GetInstanceCount()
{
	return mNumberOfInstances;
}

int ModelClass::GetVertexCount()
{
	return mVertexCount;
}

void ModelClass::SetWorldMatrix(const XMMATRIX &worldMatrix)
{
	DirectX::XMStoreFloat4x4(&mWorldMatrix, worldMatrix);
	XMMATRIX temp = DirectX::XMLoadFloat4x4(&mWorldMatrix);

	DirectX::XMStoreFloat4x4(&mInverseWorldMatrix, XMMatrixInverse(nullptr, temp));
}

BoundingBox ModelClass::GetBoundingBox()
{
	return mBoundingBox;
}

ModelPTN* ModelClass::GetModel()
{
	return mModel;
}

#pragma endregion