struct VS_IN
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texCoord :TEXCOORD;
	float3 normal : NORMAL;
};

VS_OUT BaseVSShader(VS_IN input)
{
	VS_OUT output;

	output.position = float4(input.position, 1.0f);
	output.texCoord = input.texCoord;
	output.normal = input.normal;


	return output;
}