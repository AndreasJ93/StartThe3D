#ifndef GRAPHICSCLASS_H
#define GRAPHICSCLASS_H

//For easy access
#define FULL_SCREEN false
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define VSYNC_ENABLED true
#define SCREEN_DEPTH 1000.0f
#define SCREEN_NEAR 0.05f

#include "DirectX Class.h"
#include "Camera Class.h"
#include "Input Class.h"
#include "Model Class.h"
#include "Base Shader Class.h"


class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass& original);
	~GraphicsClass();

	bool Initialize(int width, int height, HWND handle);
	void Shutdown();
	bool Frame(InputClass *mInput, float deltaTime);
	void SetCurrentMousePosition(int x, int y);
	

private:
	int mWidth, mHeight;

	bool Render();
	void Picking();

	DirectXClass* mDirect3D;

	//Needed for the camera
	CameraClass* mCamera;
	POINT mCurrentMousePosition;

	ModelClass* mModel;

	BaseShaderClass* mBaseShader;

	HWND mHandle;

};

#endif