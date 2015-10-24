#ifndef MODELCLASS_H
#define MODELCLASS_H

#include "Structs.h"
#include "Texture Class.h"

class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass& original);
	~ModelClass();

	bool Initialize(ID3D11Device* mDevice, ID3D11DeviceContext* mDeviceContext, WCHAR* textureFileName, char* modelFileName);
	void Shutdown();
	void Render(ID3D11DeviceContext* mDeviceContext);
	void SetWorldMatrix(const XMMATRIX &worldMatrix);
	XMMATRIX GetWorldMatrix();
	XMMATRIX GetInverseWorldMatrix();
	int GetInstanceCount();
	int GetVertexCount();
	BoundingBox GetBoundingBox();
	ModelPTN* GetModel();

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:

	//Functions
	bool InitializeBuffers(ID3D11Device* mDevice);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* mDeviceContext);
	bool LoadTexture(ID3D11Device* mDevice, ID3D11DeviceContext* mDeviceContext, const WCHAR* fileName);
	void ReleaseTexture();


	//Variables
	ID3D11Buffer *mVertexBuffer, *mIndexBuffer;
	int mVertexCount, mIndexCount, mNumberOfInstances;
	ModelPTN* mModel;
	TextureClass* mTexture;
	XMFLOAT4X4 mWorldMatrix;
	XMFLOAT4X4 mInverseWorldMatrix;
	BoundingBox mBoundingBox;
	bool mGotTexture;
	wstring mTextureString;

	//Read Model
	bool LoadModel(char* fileName);
	void ReleaseModel();

	bool ReadFileCounts(char* fileName, int& vertexCount, int& textureCount, int& normalCount, int& faceCount);
	bool LoadDataStructures(char* fileName, int vertexCount, int textureCount, int normalCount, int faceCount);


};




#endif