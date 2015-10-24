#ifndef CAMERACLASS_H
#define CAMERACLASS_H

#include "Help Functions and Libraries.h"

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass& original);
	~CameraClass();

	void Shutdown();

	//Get and Set functions
	void SetPosition(XMFLOAT3 newPosition);
	void SetRotation(XMFLOAT3 newRotation);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
	float GetLookZ();

	XMMATRIX GetViewMatrix();

	//Keep the camera in sync with the application
	void SetFrameTime(float time);

	//Movement functions
	void MoveForward(bool keyDown);
	void MoveBackward(bool keyDown);
	void MoveLeft(bool keyDown);
	void MoveRight(bool keyDown);
	void MoveUpward(bool keyDown);
	void MoveDownward(bool keyDown);
	void TurnLeft(bool keyDown);
	void TurnRight(bool keyDown);
	void LookUpward(bool keyDown);
	void LookDownward(bool keyDown);
	void LookAround(int x, int y);

	//Render
	void Render();



private:
	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMFLOAT3 mLookAt;
	float mLookZ;
	XMFLOAT3 mUp;

	XMFLOAT4X4 mViewMatrix;

	float mFrameTime;
	float mForwardSpeed, mBackwardSpeed;
	float mLeftSpeed, mRightSpeed;
	float mUpwardSpeed, mDownwardSpeed;
	float mLeftTurnSpeed, mRightTurnSpeed;
	float mLookUpSpeed, mLookDownSpeed;

	POINT mLastMousePosition;
};






#endif