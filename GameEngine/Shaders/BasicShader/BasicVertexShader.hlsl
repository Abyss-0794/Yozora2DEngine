#include "BasicHeader.hlsli"

PSInput BasicVS( VSInput input )
{
    PSInput output;
    output.pos = float4(input.pos, 1.0f);
    output.uv = input.uv;
	
	return output;
}