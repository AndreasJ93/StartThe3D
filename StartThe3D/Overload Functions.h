#ifndef OVERLOADFUNCTIONS_H
#define OVERLOADFUNCTIONS_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;

inline XMFLOAT2 operator-(XMFLOAT2 a, XMFLOAT2 b)
{
	XMFLOAT2 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	return result;
};

inline XMFLOAT3 operator+(XMFLOAT3 a, XMFLOAT3 b)
{
	XMFLOAT3 result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
};

inline XMFLOAT3 operator+=(XMFLOAT3 a, XMFLOAT3 b)
{
	XMFLOAT3 result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
};

inline XMFLOAT3 operator-(XMFLOAT3 a, XMFLOAT3 b)
{
	XMFLOAT3 result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;

	return result;
};

inline float Dot(XMFLOAT3 a, XMFLOAT3 b)
{
	return a.x*b.x + a.y * b.y + a.z*b.z;
}

inline XMFLOAT3 operator*(XMFLOAT3 a, float b)
{
	XMFLOAT3 result;
	result.x = a.x * b;
	result.y = a.y * b;
	result.z = a.z * b;
	return result;
}

inline XMFLOAT3 operator*(float b, XMFLOAT3 a)
{
	XMFLOAT3 result;
	result.x = a.x * b;
	result.y = a.y * b;
	result.z = a.z * b;
	return result;
}




#endif
