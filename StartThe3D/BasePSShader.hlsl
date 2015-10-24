Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct GS_OUT
{
	float4 position : SV_POSITION;
	float2 texCoord :TEXCOORD;
	float3 normal : NORMAL;
};


float4 BasePSShader(GS_OUT input) : SV_TARGET
{
	float4 texColour;

	texColour = shaderTexture.Sample(SampleType, input.texCoord);

	return texColour;
}