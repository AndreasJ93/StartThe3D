#include "Graphic Class.h"

#pragma region Constructor and Deconstructor

GraphicsClass::GraphicsClass()
{
	mDirect3D = 0;
	mCamera = 0;

}


GraphicsClass::GraphicsClass(const GraphicsClass& original)
{

}


GraphicsClass::~GraphicsClass()
{

}

#pragma endregion

#pragma region Initialize and Shutdown

bool GraphicsClass::Initialize(int width, int height, HWND handle)
{
	bool result;

	mWidth = width;
	mHeight = height;
	mCurrentMousePosition.x = width / 2;
	mCurrentMousePosition.y = height / 2;

	ClientToScreen(GetActiveWindow(), &mCurrentMousePosition);

	//Create the Direct3D object
	mDirect3D = new DirectXClass;
	if (!mDirect3D)
	{
		return false;
	}

	//Initialize the object
	result = mDirect3D->Initialize(width, height, VSYNC_ENABLED, handle, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(handle, L"Could not initialize Direct3D", L"ERROR", MB_OK);
	}

	cout << ">>Direct X Initialized" << endl;

	//Initialize the basic Shader
	mBaseShader = new BaseShaderClass;
	if (!mBaseShader)
	{
		return false;
	}

	result = mBaseShader->Initialize(mDirect3D->GetDevice(), mHandle, L"BaseVSShader.hlsl", L"BaseGSShader.hlsl", L"BasePSShader.hlsl", NULL, NULL, NULL, "BaseVSShader", "BaseGSShader", "BasePSShader", NULL, NULL, NULL);
	if (!result)
	{
		return false;
	}

	cout << ">>Base Shader Initialized" << endl;

	//Create the Camera object
	mCamera = new CameraClass;
	if (!mCamera)
	{
		return false;
	}

	cout << ">>Camera Initialized" << endl;

	mModel = new ModelClass;
	if (!mModel)
	{
		return false;
	}
	
	if (!mModel->Initialize(mDirect3D->GetDevice(), mDirect3D->GetDeviceContext(), NULL, "Models\\Dragon2.obj"))
	{
		return false;
	}

	mModel->SetWorldMatrix(XMMatrixIdentity());

	cout << ">>Model Loaded: Dragon" << endl;

	return true;
}

void GraphicsClass::Shutdown()
{

	if (mCamera)
	{
		delete mCamera;
		mCamera = 0;
	}

	if (mBaseShader)
	{
		mBaseShader->Shutdown();
		mBaseShader = 0;
	}

	if (mModel)
	{
		mModel->Shutdown();
		mModel = 0;
	}

	if (mDirect3D)
	{
		mDirect3D->Shutdown();
		delete mDirect3D;
		mDirect3D = 0;
	}
}

#pragma endregion

#pragma region Render functions

void GraphicsClass::SetCurrentMousePosition(int x, int y)
{
	mCurrentMousePosition.x = x;
	mCurrentMousePosition.y = y;

}

bool GraphicsClass::Frame(InputClass *mInput, float deltaTime)
{
	bool result;

	//Render the graphics scene
	mCamera->SetFrameTime(deltaTime);

	mCamera->MoveForward(mInput->IsKeyDown((unsigned int)0x57));
	mCamera->MoveBackward(mInput->IsKeyDown((unsigned int)0x53));
	mCamera->MoveLeft(mInput->IsKeyDown((unsigned int)0x41));
	mCamera->MoveRight(mInput->IsKeyDown((unsigned int)0x44));
	mCamera->MoveDownward(mInput->IsKeyDown('Z'));
	mCamera->MoveUpward(mInput->IsKeyDown((unsigned int)0x20));
	//mCamera->LookAround(mCurrentMousePosition.x, mCurrentMousePosition.y);
	
	if (mInput->IsMouseDown())
	{
		mInput->MouseUp();
		Picking();
	}

	mCamera->Render();



	result = Render();

	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render()
{
	

	mDirect3D->BeginScene();
	mModel->Render(mDirect3D->GetDeviceContext());
	mBaseShader->Render(mDirect3D->GetDeviceContext(), mModel->GetIndexCount(), mModel->GetWorldMatrix(), mCamera->GetViewMatrix(), mDirect3D->GetProjectionMatrix(), mModel->GetTexture());
	mDirect3D->EndScene();
	return true;
}

void GraphicsClass::Picking()
{
	//Take down the information required for tranfromations from NDC to Screen Space
	float a11 = XMVectorGetX(mDirect3D->GetProjectionMatrix().r[0]);
	float a22 = XMVectorGetY(mDirect3D->GetProjectionMatrix().r[1]);

	POINT temp;
	//Save down the SCREEN coordinates.
	GetCursorPos(&temp);

	//Convert them to the little window we have, so we don't have to care where on the screen the window is placed.
	//Or, well, techincally that's exactly what we do here.
	ScreenToClient(mHandle, &temp);

	//Transform these screen coordinates to View Space
	//First to Screen Space
	float vx = ((2.0f*temp.x / mWidth) - 1.0f) / a11;
	float vy = ((-2.0f*temp.y / mHeight) + 1.0f) / a22;

	XMMATRIX inverseViewProjection = XMMatrixInverse(nullptr, mCamera->GetViewMatrix());

	XMVECTOR rayOrigin, rayDirection;
	//Then to View Space
	float x = vx*XMVectorGetX(inverseViewProjection.r[0]) + vy * XMVectorGetX(inverseViewProjection.r[1]) + XMVectorGetX(inverseViewProjection.r[2]);
	float y = vx*XMVectorGetY(inverseViewProjection.r[0]) + vy * XMVectorGetY(inverseViewProjection.r[1]) + XMVectorGetY(inverseViewProjection.r[2]);
	float z = vx*XMVectorGetZ(inverseViewProjection.r[0]) + vy * XMVectorGetZ(inverseViewProjection.r[1]) + XMVectorGetZ(inverseViewProjection.r[2]);
	//Place it into a ray with origin and direction
	rayOrigin = XMVectorSet(mCamera->GetPosition().x, mCamera->GetPosition().y, mCamera->GetPosition().z, 1.0f);
	rayDirection = XMVectorSet(x, y, z, 0);
	rayDirection = XMVector3Normalize(rayDirection);


	//And once we have this ray, we create the dragons' (that we will pick) boundingboxes and transform them into the world.
	//For each of these dragons, we check if they're active, and if the ray hit. 
	//If it does, we change "hitDragon", our indicator, to the dragon hit, and calculate the distance.
	//Least distance will be kept.
	

	float distance = INFINITY;
	float minDistance = distance;
	

}

#pragma endregion

