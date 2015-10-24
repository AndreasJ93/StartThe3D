cbuffer MatrixBuffer
{
matrix WVP;
matrix worldMatrix;
matrix viewMatrix;
matrix projectionMatrix;
}

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texCoord :TEXCOORD;
	float3 normal : NORMAL;
};

struct GS_OUT
{
	float4 position : SV_POSITION;
	float2 texCoord :TEXCOORD;
	float3 normal : NORMAL;
};

[maxvertexcount(3)]
void BaseGSShader(
	in triangle VS_OUT input[3],
	inout TriangleStream< GS_OUT > output)
{
	GS_OUT element;
	
		[unroll]
		for (uint i = 0; i < 3; i++)
		{
			element.position = mul(input[i].position, WVP);
			element.texCoord = input[i].texCoord;
			element.normal = input[i].normal;
			output.Append(element);
		}



}