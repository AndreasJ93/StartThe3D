#ifndef STRUCTS_H
#define STRUCTS_H


#define NR_OF_SPOTLIGHTS 10
#define NR_OF_POINTLIGHTS 15
#define NR_OF_DIRECTIONALLIGHTS 10

#include "Help Functions and Libraries.h"

#pragma region Lights

struct PointLight
{
	XMFLOAT4 colour;
	XMFLOAT4 position_radius;
};

struct SpotLight
{
	XMFLOAT4 colour_angle;
	XMFLOAT4 position_reach;
	XMFLOAT4 lightDirection;
};

struct DirectionalLight
{
	XMFLOAT4 colour;
	XMFLOAT4 lightDirection;
};

#pragma endregion

#pragma region ModelTypes

struct ModelPTN
{
	XMFLOAT3 position;
	XMFLOAT2 textureCoordinates;
	XMFLOAT3 normal;
};

struct ModelPTNFloor
{
	XMFLOAT3 position;
	XMFLOAT2 textureCoordinates;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
};
struct ModelFaces
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
};

#pragma endregion

#pragma region Vertexes

struct Tangent
{
	float x, y, z;
};

struct LightQuad
{
	float x, y;
	float tx, ty;
};

struct BackgroundQuad
{
	float x, y, z;
	float nx, ny, nz;
};

struct FloorQuad
{
	float x, y, z;
	float tx, ty;
	float nx, ny, nz;
};

struct VertexTypeColor
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

struct VertexTypeTexture
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
};

struct VertexTypeLight
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
};

struct VertexTypeDisplacement
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
};

#pragma endregion

#pragma region Buffers

struct ShadowMatrixBufferType
{
	XMMATRIX WVP;
};

struct MatrixBufferType
{
	XMMATRIX WVP;
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

struct LightBufferType
{
	PointLight pointLights[NR_OF_POINTLIGHTS];
	SpotLight spotLights[NR_OF_SPOTLIGHTS];
	DirectionalLight directionalLights[NR_OF_DIRECTIONALLIGHTS];
	XMFLOAT4 ambientLight;
	XMFLOAT4 numberOfLights_Point_Spot_Directional_Empty;
};

#pragma endregion

#endif
