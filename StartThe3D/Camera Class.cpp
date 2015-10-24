#include "Camera Class.h"

#pragma region Constructor and Deconstructor

CameraClass::CameraClass()
{
	mPosition = XMFLOAT3(0.0f, 0.0f, -10.0f);
	mRotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mLookAt = XMFLOAT3(0.0f, 0.0f, 1.0f);
	mUp = XMFLOAT3(0.0f, 1.0f, 0.0f);

	XMStoreFloat4x4(&mViewMatrix, XMMatrixLookAtLH(XMLoadFloat3(&mPosition), XMLoadFloat3(&mLookAt), XMLoadFloat3(&mUp)));

	mFrameTime = 0.0f;

	mForwardSpeed = 0.0f;
	mBackwardSpeed = 0.0f;
	mLeftSpeed = 0.0f;
	mRightSpeed = 0.0f;
	mUpwardSpeed = 0.0f;
	mDownwardSpeed = 0.0f;
	mLeftTurnSpeed = 0.0f;
	mRightTurnSpeed = 0.0f;
	mLookUpSpeed = 0.0f;
	mLookDownSpeed = 0.0f;

	mLastMousePosition.x = 400;
	mLastMousePosition.y = 300;

	ClientToScreen(GetActiveWindow(), &mLastMousePosition);
}

CameraClass::CameraClass(const CameraClass& original)
{

}

CameraClass::~CameraClass()
{

}

#pragma endregion

#pragma region Get and Set functions

void CameraClass::SetPosition(XMFLOAT3 newPosition)
{
	mPosition = newPosition;
}

void CameraClass::SetRotation(XMFLOAT3 newRotation)
{
	mRotation = newRotation;
}

XMFLOAT3 CameraClass::GetPosition()
{
	return mPosition;
}

XMFLOAT3 CameraClass::GetRotation()
{
	return mRotation;
}

XMMATRIX CameraClass::GetViewMatrix()
{
	return XMLoadFloat4x4(&mViewMatrix);
}

float CameraClass::GetLookZ()
{
	return mLookZ;
}

#pragma endregion

#pragma region Time

void CameraClass::SetFrameTime(float time)
{
	mFrameTime = time;
}

#pragma endregion

#pragma region Movement Functions
void CameraClass::MoveForward(bool keyDown)
{
	//Update the forward speed. If we move (keyDown) we "charge up" to a certain speed. 
	//If the key is not down, we slowly slow down to 0.
	if (keyDown)
	{
		mForwardSpeed += mFrameTime * 15.0f;
		if (mForwardSpeed > (mFrameTime * 40.0f))
		{
			mForwardSpeed = mFrameTime*40.0f;
		}
	}
	else
	{
		mForwardSpeed -= mFrameTime * 10.0f;
		if (mForwardSpeed < 0.0f)
		{
			mForwardSpeed = 0.0f;
		}
	}

	float radians = XMConvertToRadians(mRotation.y);

	//Update position.
	mPosition.z += cosf(radians)*mForwardSpeed;
	mPosition.x += sinf(radians)*mForwardSpeed;

}
void CameraClass::MoveBackward(bool keyDown)
{
	//Update the backward speed. If we move (keyDown) we "charge up" to a certain speed. 
	//If the key is not down, we slowly slow down to 0.
	if (keyDown)
	{
		mBackwardSpeed += mFrameTime * 15.0f;
		if (mBackwardSpeed > (mFrameTime * 40.0f))
		{
			mBackwardSpeed = mFrameTime*40.0f;
		}
	}
	else
	{
		mBackwardSpeed -= mFrameTime * 10.0f;
		if (mBackwardSpeed < 0.0f)
		{
			mBackwardSpeed = 0.0f;
		}
	}

	float radians = XMConvertToRadians(mRotation.y);

	//Update position.
	mPosition.z -= cos(radians)*mBackwardSpeed;
	mPosition.x -= sinf(radians)*mBackwardSpeed;
}

void CameraClass::MoveLeft(bool keyDown)
{
	{
		//Update the backward speed. If we move (keyDown) we "charge up" to a certain speed. 
		//If the key is not down, we slowly slow down to 0.
		if (keyDown)
		{
			mLeftSpeed += mFrameTime * 15.0f;
			if (mLeftSpeed > (mFrameTime * 40.0f))
			{
				mLeftSpeed = mFrameTime*40.0f;
			}
		}
		else
		{
			mLeftSpeed -= mFrameTime * 10.0f;
			if (mLeftSpeed < 0.0f)
			{
				mLeftSpeed = 0.0f;
			}
		}

		float radians = XMConvertToRadians(mRotation.y);

		//Update position.
		mPosition.x -= cos(radians)*mLeftSpeed;
		mPosition.z += sinf(radians)*mLeftSpeed;

	}
}

void CameraClass::MoveRight(bool keyDown)
{
	{
		//Update the backward speed. If we move (keyDown) we "charge up" to a certain speed. 
		//If the key is not down, we slowly slow down to 0.
		if (keyDown)
		{
			mRightSpeed += mFrameTime * 15.0f;
			if (mRightSpeed > (mFrameTime * 40.0f))
			{
				mRightSpeed = mFrameTime*40.0f;
			}
		}
		else
		{
			mRightSpeed -= mFrameTime * 10.0f;
			if (mRightSpeed < 0.0f)
			{
				mRightSpeed = 0.0f;
			}
		}

		float radians = XMConvertToRadians(mRotation.y);

		//Update position.
		mPosition.x += cos(radians)*mRightSpeed;
		mPosition.z -= sinf(radians)*mRightSpeed;


	}
}
void CameraClass::MoveUpward(bool keyDown)
{
	if (keyDown)
	{
		mUpwardSpeed += mFrameTime*40.0f;
		if (mUpwardSpeed > (mFrameTime*100.0f))
		{
			mUpwardSpeed = mFrameTime * 100.0f;
		}
	}
	else
	{
		mUpwardSpeed -= mFrameTime*15.2f;
		if (mUpwardSpeed < 0.0f)
		{
			mUpwardSpeed = 0.0f;
		}
	}

	mPosition.y += mUpwardSpeed;
}
void CameraClass::MoveDownward(bool keyDown)
{
	if (keyDown)
	{
		mDownwardSpeed += mFrameTime*40.0f;
		if (mDownwardSpeed > (mFrameTime*100.0f))
		{
			mDownwardSpeed = mFrameTime * 100.0f;
		}
	}
	else
	{
		mDownwardSpeed -= mFrameTime*15.2f;
		if (mDownwardSpeed < 0.0f)
		{
			mDownwardSpeed = 0.0f;
		}
	}

	mPosition.y -= mDownwardSpeed;
}
void CameraClass::TurnLeft(bool keyDown)
{
	if (keyDown)
	{
		mLeftTurnSpeed += mFrameTime*0.5f;
		if (mLeftTurnSpeed > (mFrameTime*3.5f))
		{
			mLeftTurnSpeed = mFrameTime * 3.5f;
		}
	}
	else
	{
		mLeftTurnSpeed -= mFrameTime*0.05f;
		if (mLeftTurnSpeed < 0.0f)
		{
			mLeftTurnSpeed = 0.0f;
		}
	}

	mRotation.y += mLeftTurnSpeed;

	if (mRotation.y < 0.0f)
	{
		mRotation.y += 360.0f;
	}
}
void CameraClass::TurnRight(bool keyDown)
{
	if (keyDown)
	{
		mRightTurnSpeed += mFrameTime*0.5f;
		if (mRightTurnSpeed >(mFrameTime*3.5f))
		{
			mRightTurnSpeed = mFrameTime * 3.5f;
		}
	}
	else
	{
		mRightTurnSpeed -= mFrameTime*0.05f;
		if (mRightTurnSpeed < 0.0f)
		{
			mRightTurnSpeed = 0.0f;
		}
	}

	mRotation.y -= mRightTurnSpeed;

	if (mRotation.y > 0.0f)
	{
		mRotation.y -= 360.0f;
	}
}
void CameraClass::LookUpward(bool keyDown)
{
	if (keyDown)
	{
		mLookUpSpeed += mFrameTime*0.1f;

		if (mLookUpSpeed > (mFrameTime*1.5f))
		{
			mLookUpSpeed = 1.55f;
		}
	}
	else
	{
		mLookUpSpeed -= mFrameTime*0.5f;

		if (mLookUpSpeed < 0.0f)
		{
			mLookUpSpeed = 0.0f;
		}
	}

	mRotation.x -= mLookUpSpeed;

	if (mRotation.x > 90.0f)
	{
		mRotation.x = 90.0f;
	}
}
void CameraClass::LookDownward(bool keyDown)
{
	if (keyDown)
	{
		mLookDownSpeed += mFrameTime*5.0f;

		if (mLookDownSpeed > (mFrameTime*15.0f))
		{
			mLookDownSpeed = 1.5f;
		}
	}
	else
	{
		mLookDownSpeed -= mFrameTime*0.05f;

		if (mLookDownSpeed < 0.0f)
		{
			mLookDownSpeed = 0.0f;
		}
	}

	mRotation.x += mLookDownSpeed;

	if (mRotation.x < -90.0f)
	{
		mRotation.x = -90.0f;
	}
}

void CameraClass::LookAround(int x, int y)
{
	float dy = XMConvertToRadians(floor(static_cast<float>(x - mLastMousePosition.x) / 45));
	float dx = XMConvertToRadians(floor(static_cast<float>(y - mLastMousePosition.y) / 45));


	mRotation.x += dx*mFrameTime * 1000;
	mRotation.y += dy*mFrameTime * 1000;

	if (mRotation.x > 90.0f)
	{
		mRotation.x = 90.0f;
	}
	if (mRotation.x < -90.0f)
	{
		mRotation.x = -90.0f;
	}
	if (mRotation.y > 0.0f)
	{
		mRotation.y -= 360.0f;
	}
	if (mRotation.y < 0.0f)
	{
		mRotation.y += 360.0f;
	}



}

#pragma region Render

void CameraClass::Render()
{
	XMVECTOR up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	up = XMLoadFloat3(&mUp);
	position = XMLoadFloat3(&mPosition);
	lookAt = XMLoadFloat3(&mLookAt);

	pitch = XMConvertToRadians(mRotation.x);
	yaw = XMConvertToRadians(mRotation.y);
	roll = XMConvertToRadians(mRotation.z);

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	up = XMVector3TransformNormal(up, rotationMatrix);
	lookAt = XMVector3TransformNormal(lookAt, rotationMatrix);

	mLookZ = XMVectorGetByIndex(lookAt, 2);
	lookAt = position + lookAt;

	XMStoreFloat4x4(&mViewMatrix, XMMatrixLookAtLH(position, lookAt, up));


}


#pragma endregion