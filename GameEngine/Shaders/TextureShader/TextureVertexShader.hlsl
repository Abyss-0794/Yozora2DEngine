#include "TextureHeader.hlsli"

PSInput TexVS( VSInput input )
{
    PSInput output;
    output.pos = float4(input.pos, 1.0f);
    output.uv = input.uv;
	return output;
}